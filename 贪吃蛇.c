#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <curses.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>
#ifndef TIOCGWINSZ
#include <sys/ioctl.h>
#endif

#define ERRBUFSIZE	128
#define DEFSNAKE	'o' //蛇身字符
#define DEFFOOD		'x' //食物字符
#define DEFLINES	33 //窗口行
#define DEFCOLS		80 //窗口列
#define DEFHL		3 //底栏
#define DEFVL		12 //侧边栏
#define DEFINTERVAL	20000 //间隔时间20ms
#define DEFMINSLEN	4 //默认蛇长
#define DEFMAXSLEN	5 //最大蛇长
#define DEFFLEN		3 //食物数量
#define TIMEBUFSIZE	32

struct snake //蛇身坐标
{
	int	sk_l;
	int	sk_c;
};

struct food //食物坐标
{
	int	fd_l;
	int	fd_c;
	int	fd_used;
};

struct gameobj //游戏数据
{
	struct snake * obj_snake;
	struct food * obj_food;
	int	obj_snakelen;
	int	obj_lines;
	int	obj_cols;
	int	obj_score;
	time_t	obj_ptim;
};

struct gamesetting //游戏设置
{
	int	wlines;
	int	wcols;
	int	hl;
	int	vl;
	int	interval;
	int	hz; //每秒中断次数
	int	fullout; //中断次数大或等于fullout则中断次数置0
	int	statflag; //当前状态
	int	minslen; //初始蛇长
	int	maxslen;
	int	flen; //食物数量
	WINDOW * mwin; //游戏窗口
};

struct gameobj obj;
struct gamesetting st;
int pcomm,comm; //pcomm: 上一次输入字符 comm: 当前输入的字符
/*
obj.statflag是游戏状态
0: 停止
1: 运行
2: 暂停
*/

static void
err_exit(const char * fmt, ...)
{
	char buf[ERRBUFSIZE];
	va_list va;
	va_start(va,fmt);
	vsnprintf(buf,ERRBUFSIZE,fmt,va);
	delwin(st.mwin);
	endwin();
	fprintf(stderr,"%s\n",buf);
	exit(1);
}

static inline void *
malloc_clean(int c, size_t count)
{
	void *p = malloc(count);
	return memset(p, c, count);
}

void
do_exit(void)
{
	delwin(st.mwin);
	endwin();
	exit(0);
}

WINDOW *
draw_win(WINDOW * pwin, int slines, int scols)
{
	WINDOW * win;
	if((win = subwin(pwin,st.wlines,st.wcols,slines,scols)) == NULL)
		return NULL;
	box(win,'|','-'); //画边框
	mvwhline(win,st.wlines-3,1,'-',st.wcols-2); //画底栏
	mvwvline(win,1,st.wcols-12,'|',st.wlines-4); //画侧边栏
	return win;
}

void
clear_win(void)
{
	int i,j;
	for(i = 1;i < obj.obj_lines;i++)
		for(j = 1;j < obj.obj_cols;j++)
			mvwaddch(st.mwin,i,j,' ');
	wrefresh(st.mwin);
}

int
test_comm(int ch)  //检查输入的字符
{
	if(ch != 'w' && ch != 'a' && ch != 's' && ch != 'd')
		return 0;
	if(pcomm == 'w' && ch == 's')
		return 0;
	if(pcomm == 'a' && ch == 'd')
		return 0;
	if(pcomm == 's' && ch == 'w')
		return 0;
	if(pcomm == 'd' && ch == 'a')
		return 0;
	return 1;
}

void
print_label(int lno) //写底栏文字
{
	const char * cptr = NULL;
	switch(lno)
	{
		case -1:
			cptr = "                                                         ";
			break;
		case 0:
			cptr = "Enter control + c to pause";
			break;
		case 1:
			cptr = "Enter c to continue, q to quit, control + z to minimize";
			break;
	}
	if(cptr)
		mvwprintw(st.mwin,st.wlines-2,1,cptr);
	wrefresh(st.mwin);
}

int
cmp_snake(int lines, int cols) //坐标与蛇坐标对比
{
	int i;
	for(i = 0;i < obj.obj_snakelen;i++)
	{
		if(lines == obj.obj_snake[i].sk_l)
			if(cols == obj.obj_snake[i].sk_c)
				return 0;
	}
	return 1;
}

int
cmp_food(int lines, int cols) //坐标与食物坐标对比
{
	int i;
	for(i = 0;i < st.flen;i++)
	{
		if(obj.obj_food[i].fd_used == 0)
			continue;
		if(lines == obj.obj_food[i].fd_l)
			if(cols == obj.obj_food[i].fd_c)
				return 0;
	}
	return 1;
}

int
cmp_border(int lines, int cols) //坐标与边界坐标对比
{
	if(lines < 1 || lines >= obj.obj_lines)
		return 0;
	if(cols < 1 || cols >= obj.obj_cols)
		return 0;
	return 1;
}

void
generate_snake(void) //初始化蛇坐标
{
	int i,nlines,ncols;
	nlines = st.wlines / 2 - st.minslen - 3;
	ncols = st.wcols / 2;
	if((obj.obj_snake = malloc(sizeof(struct snake) * st.maxslen)) == NULL)
		err_exit("malloc error");
	for(i = st.minslen - 1;i >= 0;i--,nlines++)
	{
		obj.obj_snake[i].sk_l = nlines;
		obj.obj_snake[i].sk_c = ncols;
	}
	obj.obj_snakelen = st.minslen;
	pcomm = 'w';
}

struct food *
locate_food(int lines, int cols) //通过坐标返回食物的指针
{
	int i;
	for(i = 0;i < st.flen;i++)
		if(lines == obj.obj_food[i].fd_l)
			if(cols == obj.obj_food[i].fd_c)
				return &(obj.obj_food[i]);
	return NULL;
}

void
rand_food(void) //随机生成食物
{
	int i,cols,lines;
	if(obj.obj_food == NULL && 
		(obj.obj_food = malloc_clean(0,sizeof(struct food) * st.flen)) == NULL)
		err_exit("malloc error");
	for(i = 0;i < st.flen;i++)
	{
		if(obj.obj_food[i].fd_used == 1)
			continue;
		while(1)
		{
			lines = 1 + rand() % (obj.obj_lines - 1);
			cols = 1 + rand() % (obj.obj_cols - 1);
			if(cmp_snake(lines,cols))
				if(cmp_food(lines,cols))
				{
					obj.obj_food[i].fd_l = lines;
					obj.obj_food[i].fd_c = cols;
					obj.obj_food[i].fd_used = 1;
					break;
				}
		}
	}
}

void
init_obj(void) 
{
	obj.obj_lines = st.wlines - st.hl;
	obj.obj_cols = st.wcols - st.vl;
	obj.obj_snake = obj.obj_food = NULL;
	obj.obj_ptim = 0;
	obj.obj_score = 0;
	generate_snake();
	rand_food();
}

void
draw_game(void) //在窗口绘制食物与蛇
{
	int i;
	for(i = 0;i < st.flen;i++)
		mvwaddch(st.mwin,obj.obj_food[i].fd_l,
			obj.obj_food[i].fd_c,DEFFOOD);
	for(i = 0;i < obj.obj_snakelen;i++)
		mvwaddch(st.mwin,obj.obj_snake[i].sk_l,
			obj.obj_snake[i].sk_c,DEFSNAKE);
}

void
clear_obj(void) //在窗口上清除蛇与食物
{
	int i;
	for(i = 0;i < obj.obj_snakelen;i++)
		mvwaddch(st.mwin,obj.obj_snake[i].sk_l,
			obj.obj_snake[i].sk_c,' ');
	for(i = 0;i < st.flen;i++)
		mvwaddch(st.mwin,obj.obj_food[i].fd_l,
			obj.obj_food[i].fd_c,' ');
}

void
move_snake(int comm) //根据坐标移动蛇的坐标
{
	int i,lines,cols,bakl,bakc;
	struct food *fdp = NULL;
	lines = obj.obj_snake[obj.obj_snakelen - 1].sk_l;
	cols = obj.obj_snake[obj.obj_snakelen - 1].sk_c;
	switch(comm)
	{
		case 'w':
			lines--;
			break;
		case 'a':
			cols--;
			break;
		case 's':
			lines++;
			break;
		case 'd':
			cols++;
			break;
	}
	if(cmp_border(lines,cols) == 0)
	{
		st.statflag = 0;
		return;
	}
	if(cmp_snake(lines,cols) == 0)
	{
		st.statflag = 0;
		return;
	}
	if((fdp = locate_food(lines,cols)) != NULL && obj.obj_snakelen < st.maxslen)
	{
		obj.obj_snake[obj.obj_snakelen].sk_l = lines;
		obj.obj_snake[obj.obj_snakelen].sk_c = cols;
		fdp->fd_used = 0;
		obj.obj_snakelen++;
		rand_food();
		obj.obj_score++;
	}
	else
	{
		if(fdp)
		{
			fdp->fd_used = 0;
			obj.obj_score++;
			rand_food();
		}
		for(i = obj.obj_snakelen - 1;i >= 0;i--)
		{
			bakl = obj.obj_snake[i].sk_l;
			bakc = obj.obj_snake[i].sk_c;
			obj.obj_snake[i].sk_l = lines;
			obj.obj_snake[i].sk_c = cols;
			lines = bakl;
			cols = bakc;
		}
	}
}

void
draw_timeandscore(int f) //在侧边栏显示当前时间与游戏时间
{
	struct timespec ts;
	struct tm * t;
	time_t ti;
	static int rtflag  = 0;
	static int ptflag = 0;
	static int pflag = 0;
	char buf[TIMEBUFSIZE];
	if(f != 0)
	{
		rtflag = 0;
		ptflag = 0;
		pflag = 0;
		mvwprintw(st.mwin,9,st.wcols-10,"         ");
		mvwprintw(st.mwin,10,st.wcols-10,"         ");
		mvwprintw(st.mwin,15,st.wcols-9,"      ");
		mvwprintw(st.mwin,16,st.wcols-8,"   ");
		wrefresh(st.mwin);
		return;
	}
	if(clock_gettime(CLOCK_REALTIME,&ts) < 0)
		err_exit("clock_gettime error");
	if((t = localtime(&(ts.tv_sec))) == NULL)
		err_exit("localtime error");
	if(strftime(buf,TIMEBUFSIZE,"%T",t) <= 0)
		err_exit("strftime error");
	if(rtflag == 0)
	{
		mvwprintw(st.mwin,3,st.wcols-8,"time");
		rtflag = 1;
	}
	mvwprintw(st.mwin,4,st.wcols-10,"%s",buf);
	if(st.statflag == 0)
	{
		wrefresh(st.mwin);
		return;
	}
	if(ptflag == 0)
	{
		mvwprintw(st.mwin,9,st.wcols-10,"playtime");
		ptflag = 1;
	}
	ti = obj.obj_ptim - 1;
	if((t = gmtime(&ti)) == NULL)
		err_exit("localtime error");
	if(strftime(buf,TIMEBUFSIZE,"%T",t) <= 0)
		err_exit("strftime error");
	mvwprintw(st.mwin,10,st.wcols-10,"%s",buf);
	if(pflag == 0)
	{
		mvwprintw(st.mwin,15,st.wcols-9,"points");
		pflag = 1;
	}
	wrefresh(st.mwin);
}

void
sig_alrm(int signo)  //中断函数
{
	static int count; //记录中断次数
	int i;
	if(++count > st.fullout - 1)
		count = 0;
	if(st.statflag == 1)
	{
		if(count % st.hz == 0)
			obj.obj_ptim++;
		if(count % (st.hz / 4) == 0) //修改这里控制蛇的移动速度
		{
			clear_obj();
			if(comm)
			{
				move_snake(comm);
				pcomm = comm;
				comm = 0;
			}
			else
				move_snake(pcomm);
			mvwprintw(st.mwin,16,st.wcols-8,"%3d",obj.obj_score); //显示分数
			draw_game();
			if(st.statflag == 0) //显示game over
			{
				mvwprintw(st.mwin,obj.obj_lines / 2 - 1,
					obj.obj_cols / 2 - 5," game over ");
				mvwprintw(st.mwin,obj.obj_lines / 2,
					obj.obj_cols / 2 - 13,
					" press any key to continue ");
			}
			wrefresh(st.mwin);
		}
	}
	if(count % st.hz == 0) //每秒更新一次时间
			draw_timeandscore(0);
}

void
sig_int(int signo) //用与暂停游戏
{
	if(st.statflag == 0 || st.statflag == 2)
		do_exit();
	if(st.statflag == 1)
	{
		print_label(-1);
		print_label(1);
		st.statflag = 2;
	}
	return;
}

void
defsetting(void) //载入默认设置
{
	st.wlines = DEFLINES;
	st.wcols = DEFCOLS;
	st.hl = DEFHL;
	st.vl = DEFVL;
	st.interval = DEFINTERVAL;
	st.hz = 1000000 / DEFINTERVAL;
	st.fullout = st.hz * 6;
	st.minslen = DEFMINSLEN;
	st.maxslen = DEFMAXSLEN;
	st.flen = DEFFLEN;
	st.statflag = 0;
}

int
test_winsize(int *flines, int *fcols) //检查tty的尺寸是否满足要求 返回绘制窗口的起始位置
{
	struct winsize ws;
	if(ioctl(STDIN_FILENO,TIOCGWINSZ,(char *)&ws) < 0)
		return errno;
	if(ws.ws_row < *flines)
		return -1;
	else
		*flines = (ws.ws_row / 2 - (*flines) / 2);
	if(ws.ws_col < *fcols)
		return -1;
	else
		*fcols = (ws.ws_col /2 - (*fcols) / 2);
	return 0;
}

void
INIT(void) //初始化tty，绘制窗口，初始化间隔定时器
{
	int t1,t2;
	struct itimerval it;
	t1 = st.wlines;
	t2 = st.wcols;
	it.it_value.tv_sec = it.it_interval.tv_sec = 0;
	it.it_value.tv_usec = it.it_interval.tv_usec = st.interval;
	if(test_winsize(&t1,&t2) != 0)
		err_exit("can't run with this winsize");
	if(signal(SIGALRM,sig_alrm) == SIG_ERR)
		err_exit("signal error");
	if(signal(SIGINT,sig_int) == SIG_ERR)
		err_exit("signal error");
	srand(time(NULL));
	initscr();
	curs_set(0);
	cbreak();
	noecho();
	if((st.mwin = draw_win(stdscr,t1,t2)) == NULL)
		err_exit("draw_win error");
	draw_timeandscore(0);
	wrefresh(st.mwin);
	if(setitimer(ITIMER_REAL,&it,NULL) < 0)
		err_exit("setitimer error: %s",strerror(errno));
}

void
print_menu(int no) //显示菜单
{
	const char * p1, * p2;
	int nlines,ncols;
	p1 = p2 = NULL;
	switch(no)
	{
		case -1:
			p1 = "                    ";
			p2 = "                    ";
			break;
		case 0:
			p1 = "1.start game";
			p2 = "q.quit";
			break;
		case 1:
			p1 = "1.restart game";
			p2 = "q.quit";
			break;
	}
	if(p1 && p2)
	{
		nlines = (st.wlines - 3) / 2 - 1;
		ncols = (st.wcols - 12) / 2 -
			(strlen(p1) > strlen(p2)?strlen(p1):strlen(p2)) / 2;
		mvwprintw(st.mwin,nlines,ncols,p1);
		mvwprintw(st.mwin,nlines+1,ncols,p2);
	}
	wrefresh(st.mwin);
}

void
do_game(void) 
{
	int ch;
	st.statflag = 1;
	while(st.statflag)
	{
		ch = getch();
		if(st.statflag == 2)
		{
			if(ch == 'c')
			{
				print_label(-1);
				print_label(0);
				st.statflag = 1;
				continue;
			}
			if(ch == 'q')
				do_exit();
		}
		else if(test_comm(ch))
			comm = ch;
	}
}				

void
do_start(int no) //处理菜单对应的选项
{
	int ch;
	clear_win();
	print_menu(no);
	if(no)
	{
		free(obj.obj_snake);
		free(obj.obj_food);
	}
	while(1)
	{
		ch = getch();
		if(ch == ERR)
			err_exit("getch error");
		if(ch == '1')
			break;
		if(ch == 'q')
			do_exit();
	}
	print_menu(-1);
	init_obj();
	print_label(0);
	do_game();
	print_label(-1);
	draw_timeandscore(1);
}

int
main(int argc, char * argv[])
{
	defsetting(); //载入默认设置
	INIT(); //初始化
	print_menu(0); //显示菜单
	do_start(0); //开始游戏
	while(1)
		do_start(1); //失败后能重新开始
	endwin();
	exit(0);
}
