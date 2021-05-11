//成功版本1
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
 
	printf("%2d ",buf.st_nlink);    //打印文件的硬链接数
	printf("%-5s",psd->pw_name);    //打印用户的名字
	printf("%-5s", grp->gr_name);   //打印用户组的名字
 
	printf("%6d", buf.st_size);     //打印文件大小
	strcpy(buf_time,ctime(&buf.st_mtime));//把时间转换成普通表示格式
 
	buf_time[strlen(buf_time)-1]='\0';    //去掉换行符
	printf("  %s", buf_time);//输出时间 
    sprintf(colorname,"\033[%dm%s\033[0m",filecolor,name);
	printf(" %-s\n", colorname);
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
		g_leave_len=MAXROWLEN;
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
	printf("%d ", buf.st_ino);
	sprintf(colorname,"\033[%dm%s\033[0m",filecolor,name);
	printf(" %-s", colorname);
	//输出少于要求，补够空格
    for(i=0;i<len+5;i++)
		printf(" ");
	g_leave_len = g_leave_len - 45;
	if(g_leave_len < 45)
	{
		printf("\n");
		g_leave_len=MAXROWLEN;
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
	//printf("aaa%saaa\n",pathname);
	if(lstat(pathname, &buf) == -1)
	{
		printf("%s无法打开\n", pathname);
		return ;
		//my_err("stat",__LINE__);
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
				display_single(name,filecolor);
			}
			break;
 
		case PARAM_I:
			if(name[0]!='.')
            {
				display_st_ino(buf,name,filecolor);
			}
			break;

        case PARAM_S:
            if(name[0]!='.')
            {
				printf("%2d  ",buf.st_blocks/2);
                display_single(name,filecolor);
            }
            break;

		case PARAM_A:
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
			display_st_ino(buf,name,filecolor);
			break;

        case PARAM_A+PARAM_S:
			printf("%2d  ",buf.st_blocks/2);
            display_single(name,filecolor);
            break;

        case PARAM_L+PARAM_S:
            if(name[0]!='.')
            {
                printf("%d  ",buf.st_blocks/2);
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
            if(name[0]!='.')
            {
                printf("%2d  ",buf.st_ino);
				printf("%2d  ",buf.st_blocks/2);            
                display_single(name,filecolor);
            }
            break;

        case PARAM_A+PARAM_L+PARAM_S:
            printf("%d  ",buf.st_blocks/2);
            display_attribute(buf, name,filecolor);

            break;

        case PARAM_A+PARAM_I+PARAM_S:
            printf("%d  ",buf.st_ino); 
			printf("%2d  ",buf.st_blocks/2);              
            display_single(name,filecolor);
            break;

        case PARAM_L+PARAM_I+PARAM_S:
            if(name[0] != '.')
            {
                printf("%d\t",buf.st_ino);
                printf("%d\t",buf.st_blocks/2);
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
	//char filename[256][PATH_MAX+1];
    char temp[PATH_MAX+10];
	//long filetime[256][1];
	flag_param_temp = flag_param;
	dir = opendir(path);


	if(dir == NULL)
	{
		printf("%s:无法打开\n",path);
  		//my_err("oppendir",__LINE__);
		return ;
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
		len = a/3*2 + b;
		if(g_maxlen<len)
		{
			g_maxlen = len;
		}
		count++;                                 //文件个数
	}
    
	han = g_leave_len/(g_maxlen+15);
	if(g_maxlen >40)
	{
		han = 1;
	}

	closedir(dir);
	//printf("aaa%dAAA\n", count);
	//if(count>256)
    //    printf("%d :too many files under this dir",__LINE__);
    //char filename[count][PATH_MAX+1];
    //long filetime[count][1];
	char **filename = (char **)malloc(sizeof(char*)*count);
	long **filetime = (long **)malloc(sizeof(long*)*count);
	len = strlen(path);
	dir = opendir(path);
	//得到该目录下的所有文件的路径
	for(i = 0;i < count ;i++)
	{
		filename[i] = (char *)malloc(sizeof(char)*1000);
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
			filetime[i] = (long*)malloc(sizeof(long));
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
	else if(flag_param & PARAM_R)//根据名字排序
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
                    if(filename[i][2] != '.')
                    {
                        stat(filename[i],&name);
                        total = total + name.st_blocks/2;
                    }
                }
            }
            printf("总用量: %d\n", total);
        }
		if(flag_param & PARAM_RR)//递归输出
		{
			flag_param -= PARAM_RR;
			for(i = count-1;i >= 0;i--)
			{
				lstat(filename[i],&buf);
				if(S_ISDIR(buf.st_mode))
				{
					len = strlen(filename[i]);
					if(filename[i][len-1]=='.' && filename[i][len-2] == '/'
					    || filename[i][len-1] == '.' && filename[i][len-2] == '.' &&
						filename[i][len-3]=='/')
					{
						continue;
					}
					if(!(flag_param & PARAM_A))
					{
						if(filename[i][2] == '.')
							continue;
					}
					printf("\n\n%s :\n",filename[i]);
					len = strlen(filename[i]);
					filename[i][len] = '/';
					filename[i][len+1] = '\0';
					display_dir(flag_param,path);
				}
				else
				{
					display(flag_param,filename[i]);
				}
			}
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
            printf("总用量: %d\n", total);
        }
		if(flag_param & PARAM_RR)
		{
			flag_param -= PARAM_RR;
    		for (i = 0; i < count; i++)
			{
				lstat(filename[i],&buf);
				if(S_ISDIR(buf.st_mode))
				{
					len=strlen(filename[i]);
					if(filename[i][len-1]=='.' && filename[i][len-2]=='/'
						||filename[i][len-1] == '.' && filename[i][len-2]=='.'&&
						filename[i][len-3]=='/') 
						continue;
					if(!(flag_param & PARAM_A))
					{
						if(filename[i][2] == '.')
							continue;
					}
					
					printf("\n\n%s :\n",filename[i]);
					len=strlen(filename[i]);
					filename[i][len]='/';
					filename[i][len+1]='\0';		
					display_dir(flag_param_temp,filename[i]);
				}
				else
					display(flag_param,filename[i]);
			}
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
//段错误