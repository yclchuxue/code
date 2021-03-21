#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    struct stat buf;
	char *name;
    if(stat("./test.c", &buf) == -1)
    {
        perror("test.c");
        exit(1);
    }
	//scanf("%s",&name);
	//lstat(name, &buf);
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
	
	if(buf.st_mode & S_IXUSR)
		printf("x");
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
	
	if(buf.st_mode & S_IXGRP)
		printf("x");
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
	
	if(buf.st_mode & S_IXOTH)
		printf("x");
	else 
		printf("-");


	return 0;
}