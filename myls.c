#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <linux/limits.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <errno.h>
#include <signal.h>

#define PARAM_NONE 0
#define PARAM_A    1              //参数a
#define PARAM_L    2              //参数l
#define PARAM_I    4              //参数i
#define PARAM_R    8              //参数r
#define PARAM_T    16             //参数t
#define PARAM_RR   32             //参数R
#define PARAM_S    64             //参数s
#define MAXROWLEN  155            //每行所用最大格数
 
int g_maxlen;                     //最长文件名长度
int g_leave_len = MAXROWLEN;
int total = 0;                    //文件的大小总和
int h = 0;                        //每行已输出文件名的个数，用来判断是否换行
int han = 4;                      //一行输出文件名的个数
int y;                        //R递归时使用
 
/******************错误分析********************/
void my_err(const char *err_string,int line)
{
	fprintf(stderr, "line:%d ", line);
	perror(err_string);
	exit(1);
}

/*********************************************
 * 功能 ：输出文件信息  参数  -l
 * 参数1  文件信息stat结构体
 * 参数2  文件名name
 * 参数3  文件显示颜色filecolor
 *********************************************/
void display_attribute(struct stat buf,char * name,int filecolor)
{
    char colorname[NAME_MAX + 30];
	char buf_time[32];
	struct passwd *psd;
	struct group  *grp;
/****************文件类型**********************/
	if(S_ISLNK(buf.st_mode)){
		printf("l");
	} else if(S_ISREG(buf.st_mode)){
		printf("-");
	} else if(S_ISDIR(buf.st_mode)){
		printf("d");
	} else if(S_ISCHR(buf.st_mode)){
		printf("c");
	} else if(S_ISBLK(buf.st_mode)){
		printf("b");
	} else if(S_ISFIFO(buf.st_mode)){
		printf("f");
	} else if(S_ISSOCK(buf.st_mode)){
		printf("s");
	}
 
 
/********************文件权限**************************/ 
 
    //拥有者权限
    if(buf.st_mode & S_IRUSR)   
		printf("r");
	else 
		printf("-");
	if(buf.st_mode & S_IWUSR)
		printf("w");
	else 
		printf("-");
	
	if(buf.st_mode & S_IXUSR){
		printf("x");
    }
	else 
		printf("-");
	
    //组权限	
	if(buf.st_mode & S_IRGRP)
		printf("r");
	else 
		printf("-");
 
    if(buf.st_mode & S_IWGRP)
		printf("w");
	else 
		printf("-");
	
	if(buf.st_mode & S_IXGRP){
		printf("x");
 
    }
	else 
		printf("-");
	
 
    //其他用户权限
	if(buf.st_mode & S_IROTH)
		printf("r");
	else 
		printf("-");
	
    if(buf.st_mode & S_IWOTH)
		printf("w");
	else 
		printf("-");
	
	if(buf.st_mode & S_IXOTH){
		printf("x");
    }
	else 
		printf("-");
	
 
    printf("\t");	
    //通过用户和组id得到用户的信息和其所在组的信息
	psd = getpwuid(buf.st_uid);
	grp = getgrgid(buf.st_gid);
 
	printf("%4d ",buf.st_nlink);    //打印文件的硬链接数
	printf("%-8s",psd->pw_name);    //打印用户的名字
	printf("%-8s", grp->gr_name);   //打印用户组的名字
 
	printf("%6d", buf.st_size);     //打印文件大小
	strcpy(buf_time,ctime(&buf.st_mtime));//把时间转换成普通表示格式
 
	buf_time[strlen(buf_time)-1]='\0';    //去掉换行符
	printf("  %s", buf_time);//输出时间 
    sprintf(colorname,"\033[%dm%s\033[0m",filecolor,name);
	printf(" %-s\n", colorname);
}
 
/******************************
*功能：输出文件信息  参数  -s
*参数1  文件名name
*参数2  输出文件颜色filecolor
******************************/

void display_s(char *name,int filecolor)
{
	char colorname[NAME_MAX + 30];
	int i,len,j = 0;
    int a = 0,b = 0;
	h++;
    //名字的长度
	len = strlen(name);
	for(i=0;i<len;i++)
	{
		if(name[i] < 0)
		{
			j++;
		}
	}

	len = g_maxlen - len + j/3;
    struct stat buf;
    if(stat(name,&buf) == -1)
    {
        perror("name");
        exit(1);
    }
	sprintf(colorname,"\033[%dm%s\033[0m",filecolor,name);
    printf("%2d",buf.st_blocks/2);
	printf("  %-s", colorname);
	//输出若个个空格，补够一个单位
    for(i=0;i<len+5;i++)
		printf(" ");
	if(h == han)
	{
		printf("\n");
		h = 0;
	}
}
 
 
 
/*******************************
 * 功能 ：输出文件名
 * 参数1 文件名
 * 参数2 文件名颜色filecolor
 * *****************************/
 void display_single(char *name,int filecolor)
{
	//printf("%d\t%d\n",han,g_maxlen);
	char colorname[NAME_MAX + 30];
	int i,len,j = 0;
	len = strlen(name);
	for(i=0;i<len;i++)
	{
		if(name[i] < 0)
		{
			j++;
		}
	}
	len = len - j/3;
	//printf("%d",len);
	if(len < 40)
	{
		len = 40 - len;
	}
	else
	{
		printf("\n");
	}
	
 
	sprintf(colorname,"\033[%dm%s\033[0m",filecolor,name);
	printf("%-s", colorname);
	for(i=0;i<len+5;i++)
	{
		printf(" ");
	}
	g_leave_len = g_leave_len - 45;
	if(g_leave_len < 45)
	{
		printf("\n");
		g_leave_len = MAXROWLEN;
	}
}
 
 
/********************************************
 * 功能：输出带有i_node的文件信息    参数 -i
 * 参数1 文件信息stat结构体
 * 参数2 文件名name
 * 参数3 显示颜色filecolor
 * *****************************************/
void display_st_ino(struct stat buf,char *name,int filecolor)
{
	char colorname[NAME_MAX + 30];
	int i,len,j = 0;
	h++;
 
	printf("%d ", buf.st_ino);
	len = strlen(name);             //名字的长度
	for(i=0;i<len;i++)
	{
		if(name[i] < 0)
		{
			j++;
		}
	}
	len = g_maxlen - len + j/3;

 
	sprintf(colorname,"\033[%dm%s\033[0m",filecolor,name);
	printf(" %-s", colorname);
	//输出少于要求，补够空格
    for(i=0;i<len+5;i++)
		printf(" ");
	if( h == han)
	{
		printf("\n");
		h = 0;
	}
}


/**************************************
 * 功能：判断是否有参数a	l	i	s及其各种组合，确定输出时文字的颜色
 * 参数1 输入参数的变形flag  = flag_param
 * 参数2 文件名pathname
 * ***********************************/ 
void display(int flag, char * pathname)
{
	int filecolor = 37;
	int i,j;
	struct stat buf;       //文件信息buf
	char name[NAME_MAX + 1];
	for(i = 0,j = 0; i<strlen(pathname);i++)
	{
		if(pathname[i] == '/')
		{
			j = 0;
			continue;
		}
		name[j++] = pathname[i];
	}
	name[j] = '\0';


    //文件名颜色
	if(lstat(pathname, &buf) == -1)
	{
		my_err("stat",__LINE__);
	}
	if(S_ISLNK(buf.st_mode)){
 
	} else if(S_ISREG(buf.st_mode)){
 
	} else if(S_ISDIR(buf.st_mode)){
        filecolor=34;
	} else if(S_ISCHR(buf.st_mode)){
        filecolor=33;
	} else if(S_ISBLK(buf.st_mode)){
        filecolor=33;
	} else if(S_ISFIFO(buf.st_mode)){
 
	} else if(S_ISSOCK(buf.st_mode)){
 
	}
 
    if(filecolor == 37&&
    		( (buf.st_mode & S_IXUSR)||
    		  (buf.st_mode & S_IXGRP)||
    		  (buf.st_mode & S_IXOTH)   )  ){
    	filecolor=32;
    }
    	
 
 
 
/********************文件权限**************************/ 
 
    
	switch(flag)
	{
		case PARAM_NONE:
			if(name[0]!='.')
            {
                han = g_leave_len/(g_maxlen+15);
				display_single(name,filecolor);
			}
			break;
 
		case PARAM_I:
			if(name[0]!='.')
            {
                han = g_leave_len/(g_maxlen+15);
				display_st_ino(buf,name,filecolor);
			}
			break;

        case PARAM_S:
            if(name[0]!='.')
            {
                han = g_leave_len/(g_maxlen+15);
                display_s(name,filecolor);
            }
            break;

		case PARAM_A:
            han = g_leave_len/(g_maxlen+15);
			display_single(name,filecolor);
			break;
 
		case PARAM_L:
			if(name[0] != '.')
            {
				display_attribute(buf, name,filecolor);
			}
			break;
 
		case PARAM_A+PARAM_L:
			display_attribute(buf,name,filecolor);
			break;
 
		case PARAM_A+PARAM_I:
            han = g_leave_len/(g_maxlen+15);
			display_st_ino(buf,name,filecolor);
			break;

        case PARAM_A+PARAM_S:
            han = g_leave_len/(g_maxlen+15);
            display_s(name,filecolor);
            break;

        case PARAM_L+PARAM_S:
            if(name[0]!='.')
            {
                printf("%d\t",buf.st_blocks/2);
                display_attribute(buf, name,filecolor);
            }
            break;

		case PARAM_L+PARAM_I:
			if(name[0] != '.')
            {
				printf("%d ",buf.st_ino);
				display_attribute(buf, name,filecolor);
			}
			break;

        case PARAM_I+PARAM_S:
            han = g_leave_len/(g_maxlen+15);
            if(name[0]!='.')
            {
                printf("%2d  ",buf.st_ino);            
                display_s(name,filecolor);
            }
            break;

        case PARAM_A+PARAM_L+PARAM_S:
            printf("%d\t",buf.st_blocks/2);
            display_attribute(buf, name,filecolor);

            break;

        case PARAM_A+PARAM_I+PARAM_S:
            han = g_leave_len/(g_maxlen+15);
            printf("%d  ",buf.st_ino);               
            display_s(name,filecolor);
            break;

        case PARAM_L+PARAM_I+PARAM_S:
            if(name[0] != '.')
            {
                printf("%d\t",buf.st_ino);
                printf("%d\t",buf.st_blocks);
                display_attribute(buf, name,filecolor);
            }
            break;
 
		case PARAM_A+PARAM_L+PARAM_I:
			printf("%d ",buf.st_ino);
			display_attribute(buf,name,filecolor);
			break;

        case PARAM_A+PARAM_I+PARAM_L+PARAM_S:
        	printf("%d  ",buf.st_ino);
			printf("%d  ",buf.st_blocks/2);
			display_attribute(buf, name,filecolor);
            break;
				
		default:
			break;
	}
}

/********************************************
 * 功能：实现rR两个参数，递归实现
 * 参数1 输入参数的变形flag_param
 * 参数2 目录名fname
 * *****************************************/
void display_rR(int flag_param,char *fname)
{
    DIR *dir;
	long t;
	int count = 0,i, j, len;
	struct dirent *ptr;
	int flag_param_temp;
	struct stat  buf;
    struct stat  name;
	char filename[256][PATH_MAX+1],temp[PATH_MAX+10];
    char muluname[256][PATH_MAX+1];
	long filetime[256][1];
	dir = opendir(fname);

    if(dir == NULL)
    {
        my_err("oppendir",__LINE__);
    }
    //解析文件个数，及文件名的最长值
	while((ptr = readdir(dir)) != NULL)
	{
		int a = 0;               //用来统计汉字的个数，个数 = a/3
		int b = 0;               //用来统计非汉字的个数 b
		for(i = 0; i < strlen(ptr->d_name); i++)
		{
			if(ptr->d_name[i]< 0)
			{
				a++;
			}
			else
			{
				b++;
			}	
		}
		len = a/3 + b;
		if(g_maxlen<len)
		{
			g_maxlen = len;
		}
		count++;                                 //文件个数
	}

	closedir(dir);

	if(count>256)
        printf("%d :too many files under this dir",__LINE__);

	len = strlen(fname);
	dir = opendir(fname);
	//得到该目录下的所有文件的路径
	for(i = 0;i < count ;i++)
	{
		ptr = readdir(dir);
		if(ptr == NULL)
		{
			my_err("oppendir",__LINE__);
		}
		strncpy(filename[i],fname,len);
		/*	1.strcpy是个不安全的函数，尽量使用strncpy替代

			2.strncpy不拷贝'\0'，要注意
		*/
		filename[i][len] = '\0';
		strcat(filename[i],ptr->d_name);
		filename[i][len+strlen(ptr->d_name)] = '\0';
	}


	closedir(dir);

	//按名字排序
	for(i=0;i<count;i++)
	{
		for(j=i;j<count;j++)
		{
			if(strcmp(filename[i],filename[j])>0)
			{
				strcpy(temp,filename[i]);
				strcpy(filename[i],filename[j]);
				strcpy(filename[j],temp);
			}
		}
	}
    //计算总用量total
    if(flag_param & PARAM_A)
    {
        for(i = 0;i<count;i++)
        {
            stat(filename[i],&name);
            total = total + name.st_blocks/2;
        }
    }
    else
    {
        for(i = 0;i<count;i++)
        {
            stat(filename[i],&name);
            if(filename[i][2] != '.')
            {
                total = total + name.st_blocks/2;
            }
        }
    }
    printf("\n");
    printf("\n%s:\n",fname);
    for(i = count-1,y = 0;i>=0;i--)
    {
        stat(filename[i],&buf);
        if(S_ISDIR(buf.st_mode))
        {
            len = strlen(filename[i]);

			//-R时只有./根目录打开，其他../     ./.    等等目录不打开
            if(filename[i][len-1] == '.' && filename[i][len-2] == '/' || filename[i][len-1] == '.' && filename[i][len-2] == '.' && filename[i][len-3] == '/')
            {
                continue;
            }
            strncpy(muluname[y],filename[i],len);
            len = strlen(muluname[y]);
            muluname[y][len] = '/';
            muluname[y][len+1] = '\0';
            y++;
        }
        display(flag_param,filename[i]);
    }
	printf("%d",y);
    for(i = 0;i < y;i++)
    {
        display_rR(flag_param,muluname[i]);
    }
}


/****************************
 * 功能：实现参数R，递归实现
 * 参数1 输入参数的变形flag_param
 * 参数2 目录名fname
 ****************************/
void display_R(int flag_param,char *fname)
{
    DIR *dir;
	long t;
	int count = 0, len, i, j;
	struct dirent *ptr;
	int flag_param_temp;
	struct stat  buf;
    struct stat  name;
	char filename[256][PATH_MAX+1],temp[PATH_MAX+10];
    char muluname[256][PATH_MAX+1];
	long filetime[256][1];
	dir = opendir(fname);

    if(dir == NULL)
    {
        my_err("oppendir",__LINE__);
    }
    //解析文件个数，及文件名的最长值
	while((ptr = readdir(dir)) != NULL)
	{
		int a = 0;               //用来统计汉字的个数，个数 = a/3
		int b = 0;               //用来统计非汉字的个数 b
		for(i = 0; i < strlen(ptr->d_name); i++)
		{
			if(ptr->d_name[i]< 0)
			{
				a++;
			}
			else
			{
				b++;
			}	
		}
		len = a/3 + b;
		if(g_maxlen<len)
		{
			g_maxlen = len;
		}
		count++;              //文件个数
	}

	closedir(dir);

	if(count>256)
        printf("%d :too many files under this dir",__LINE__);

	len = strlen(fname);
	dir = opendir(fname);
	//得到该目录下的所有文件的路径
	for(i = 0;i < count ;i++)
	{
		ptr = readdir(dir);
		if(ptr == NULL)
		{
			my_err("oppendir",__LINE__);
		}
		strncpy(filename[i],fname,len);
		/*	1.strcpy是个不安全的函数，尽量使用strncpy替代

			2.strncpy不拷贝'\0'，要注意
		*/
		filename[i][len] = '\0';
		strcat(filename[i],ptr->d_name);
		filename[i][len+strlen(ptr->d_name)] = '\0';
	}


	closedir(dir);

	flag_param -= PARAM_T;
	for(i = 0;i<count;i++)
	{
		stat(filename[i],&buf);       //用buf获取文件filename[i]中的数据
		filetime[i][0] = buf.st_mtime;
	}

	for(i = 0;i<count;i++)
	{
		for(j = i;j<count;j++)
		{
			if(filetime[i][0]<filetime[j][0])
			{
				/*交换时间filetime还要叫唤文件名*/
				t = filetime[i][0];
				filetime[i][0] = filetime[j][0];
				filetime[j][0] = t;
				strcpy(temp,filename[i]);
				strcpy(filename[i],filename[j]);
				strcpy(filename[j],temp);
			}
		}
	}


    //计算总用量total
    if(flag_param & PARAM_A)
    {
        for(i = 0;i<count;i++)
        {
            stat(filename[i],&name);
            total = total + name.st_blocks/2;
        }
    }
    else
    {
        for(i = 0;i<count;i++)
        {
            stat(filename[i],&name);
            if(filename[i][2] != '.')
            {
                total = total + name.st_blocks/2;
            }
        }
    }
    printf("\n");
    printf("\n%s:\n",fname);
	for(i = 0;i<count;i++)
	{
		printf("%s\n",filename[i]);
	}
    for(i = count-1,y = 0;i>=0;i--)
    {
        stat(filename[i],&buf);
        if(S_ISDIR(buf.st_mode))
        {
            len = strlen(filename[i]);

			//-R时只有./根目录打开，其他../     ./.    等等目录不打开
            if(filename[i][len-1] == '.' && filename[i][len-2] == '/' || filename[i][len-1] == '.' && filename[i][len-2] == '.' && filename[i][len-3] == '/')
            {
                continue;
            }
            strncpy(muluname[y],filename[i],len);
            len = strlen(muluname[y]);
            muluname[y][len] = '/';
            muluname[y][len+1] = '\0';
            y++;
        }
        display(flag_param,filename[i]);
    }
	printf("%d",y);
    for(i = 0;i < y;i++)
    {
        display_rR(flag_param,muluname[i]);
    }
}


/************************************
 * 功能：实现参数R	r	t
 * 参数1 输入参数的变形flag_param
 * 参数2 要打开的目录名path
 * **********************************/
void display_dir(int flag_param,char *path)
{
	DIR *dir;
	long t;
	int count = 0;
	int i, j, len;
	struct dirent *ptr;
	int flag_param_temp;
	struct stat  buf;
    struct stat  name;
	char filename[256][PATH_MAX+1],temp[PATH_MAX+10];
	long filetime[256][1];
    char muluname[256][PATH_MAX+1];
	flag_param_temp = flag_param;
	dir = opendir(path);

    /*用来检验是否能防止ctrl + c杀死程序
    for(i = 0;i < 10;i++)
    {
        sleep(1);
    }
    */

	if(dir == NULL)
	{
		my_err("oppendir",__LINE__);
	}
	//解析文件个数，及文件名的最长值
	while((ptr = readdir(dir)) != NULL)
	{
		int a = 0;               //用来统计汉字的个数，个数 = a/3
		int b = 0;               //用来统计非汉字的个数 b
		for(i = 0; i < strlen(ptr->d_name); i++)
		{
			if(ptr->d_name[i]< 0)
			{
				a++;
			}
			else
			{
				b++;
			}	
		}
		len = a/3 + b;
		if(g_maxlen<len)
		{
			g_maxlen = len;
		}
		count++;                                 //文件个数
	}


	closedir(dir);

	if(count>256)
        printf("%d :too many files under this dir",__LINE__);
	len = strlen(path);
	dir = opendir(path);
	//得到该目录下的所有文件的路径
	for(i = 0;i < count ;i++)
	{
		ptr = readdir(dir);
		if(ptr == NULL)
		{
			my_err("oppendir",__LINE__);
		}
		strncpy(filename[i],path,len);
		/*	1.strcpy是个不安全的函数，尽量使用strncpy替代

			2.strncpy不拷贝'\0'，要注意
		*/
		filename[i][len] = '\0';
		strcat(filename[i],ptr->d_name);
		filename[i][len+strlen(ptr->d_name)] = '\0';
	}

	closedir(dir);

	//插入排序
	if(flag_param & PARAM_T)        //根据时间排序
	{
        flag_param -= PARAM_T;
		for(i = 0;i<count;i++)
		{
			stat(filename[i],&buf);       //用buf获取文件filename[i]中的数据
			filetime[i][0] = buf.st_mtime;
		}

		for(i = 0;i<count;i++)
		{
			for(j = i;j<count;j++)
			{
				if(filetime[i][0]<filetime[j][0])
				{
					/*交换时间filetime还要叫唤文件名*/
					t = filetime[i][0];
					filetime[i][0] = filetime[j][0];
					filetime[j][0] = t;
					strcpy(temp,filename[i]);
					strcpy(filename[i],filename[j]);
					strcpy(filename[j],temp);
				}
			}
		}
	}
	else//根据名字排序
	{
		for(i=0;i<count;i++)
		{
			for(j=i;j<count;j++)
			{
				if(strcmp(filename[i],filename[j])>0)
				{
					strcpy(temp,filename[i]);
					strcpy(filename[i],filename[j]);
					strcpy(filename[j],temp);
				}
			}
		}
	}

	//计算总用量total
    if(flag_param & PARAM_A)
    {
        for(i = 0;i<count;i++)
        {
            stat(filename[i],&name);
            total = total + name.st_blocks/2;
        }
    }
    else
    {
        for(i = 0;i<count;i++)
        {
            stat(filename[i],&name);
            if(filename[i][2] != '.')
            {
                total = total + name.st_blocks/2;
            }
        }
    }
    

	if(flag_param & PARAM_L)
	{
		printf("总用量： %d\n",total);
	}

	//输出文件
	if(flag_param & PARAM_R)
	{
        flag_param -= PARAM_R;
        if(flag_param & PARAM_S)
        {
            printf("总用量: %d\n", total);
        }
		if(flag_param & PARAM_RR)//递归输出
		{
			flag_param -= PARAM_RR;
            printf("./:\n");
			for(i = count-1,y = 0;i >= 0;i--)
			{
                stat(filename[i],&buf);
                if(S_ISDIR(buf.st_mode))
                {
                    len =strlen(filename[i]);

					//-R时只有./根目录打开，其他../     ./.    等等目录不打开
                    if(filename[i][len-1]=='.' && filename[i][len-2] == '/'
					    || filename[i][len-1] == '.' && filename[i][len-2] == '.' &&filename[i][len-3]=='/')
					{
						continue;
					}
                    strncpy(muluname[y],filename[i],len);
                    len = strlen(muluname[y]);
                    muluname[y][len] = '/';
                    muluname[y][len+1] = '\0';
                    y++;
                }
                display(flag_param,filename[i]);
			}
			printf("%d",y);
            for(i = 0;i<y;i++)
            {
                display_rR(flag_param,muluname[i]);
            }
            printf("\n");
		}
		else
		{
			for(i = count-1; i >= 0; i--)
			{
				display(flag_param,filename[i]);
			}
		}
	}
	else
	{
        if(flag_param & PARAM_S)
        {
            printf("总用量: %d\n", total);
        }
		if(flag_param & PARAM_RR)//递归输出
		{
			flag_param -= PARAM_RR;
            printf("./:\n");
			for(i = count-1;i >= 0;i--)
			{
                lstat(filename[i],&buf);
                if(S_ISDIR(buf.st_mode))
                {
                    len =strlen(filename[i]);

					//-R时只有./根目录打开，其他../     ./.    等等目录不打开
                    if(filename[i][len-1]=='.' && filename[i][len-2] == '/'
					    || filename[i][len-1] == '.' && filename[i][len-2] == '.' &&filename[i][len-3]=='/')
					{
						continue;
					}
                    strncpy(muluname[y],filename[i],len);
                    len = strlen(muluname[y]);
                    muluname[y][len] = '/';
                    muluname[y][len+1] = '\0';
                    y++;
                }
                display(flag_param,filename[i]);
			}
            for(i = 0;i<y;i++)
            {
                display_rR(flag_param,muluname[i]);
            }
            printf("\n");
		}
		else
		{
			for(i=0;i<count;i++)
			{
				display(flag_param,filename[i]);
			}
		}
		if((flag_param & PARAM_L) == 0)
		{
			printf("\n");
		}	
	}
	
}

/****************************
 * 功能：防止ctrl + c杀死程序
 * *************************/
void sighandler(int signum)
{

}

int main(int argc, char *argv[])
{
    int i, j, k, num;
	char path[PATH_MAX+1];
	char param[32];
	int  flag_param = PARAM_NONE;
	struct stat  buf;

    j = 0;
    num = 0;

    signal(SIGINT, sighandler);

    //将参数提取到数组param中
    for(i = 1;i<argc;i++)
    {
        if(argv[i][0] == '-')
        {
            for(k = 1;k<strlen(argv[i]);j++,k++)
            {
                param[j] = argv[i][k];
            }
            num++;
        }
    }

    //将参数变形以数字形式保存进flag_param
    for(i = 0;i < j;i++)
    {
        if(param[i] == 'a')
        {
            flag_param |= PARAM_A;
        }
        else if(param[i] == 'l')
        {
            flag_param |= PARAM_L;
        }
        else if(param[i] == 'R')
        {
            flag_param |= PARAM_RR;
        }
        else if(param[i] == 'r')
        {
            flag_param |= PARAM_R;
        }
        else if(param[i] == 't')
        {
            flag_param |= PARAM_T;
        }
        else if(param[i] == 'i')
        {
            flag_param |= PARAM_I;
        }
        else if(param[i] == 's')
        {
            flag_param |= PARAM_S;
        }
        else
        {
            printf("ls:不适用的选项 -- %c", param[i]);
            exit(1);
        }
    }


    //判断是否有目录输入，没有则打开根目录./
    if(num + 1 == argc)
    {
		strcpy(path,"./");
		path[2] = '\0';
		display_dir(flag_param,path);
		return 0;
    }

	i = 1;
	do
	{
        if(argv[i][0] == '-')
        {
			i++;
			continue;
		} 
        else 
        {
            //得到具体路径(目录名)
			strcpy(path, argv[i]);
			if(stat(path,&buf) == -1)
				my_err("stat",__LINE__);
 
	    	//判断是否为目录文件	
            if(S_ISDIR(buf.st_mode))
			{
                //如果目录最后忘记了/则加上
				if(path[strlen(argv[i]) - 1] != '/')
				{
					path[strlen(argv[i])]  = '/';
					path[strlen(argv[i])+1] = '\0';
				}
				else 
					path[strlen(argv[i])] = '\0';
			
                display_dir(flag_param,path);//按照目录输出
				i++;
			}
            else
			{
                //按照文件输出
				display(flag_param,path);
				i++;
			}
		}
	}while (i < argc);
    return 0;
}

//-Rs        出错
//-aRl       没有.和..
//-aRli      没有.和..
//altirRs    没有.和..