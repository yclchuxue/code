#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <pwd.h>
//#include <sys/wait.h>
//#include <fcntl.h>
//#include <dirent.h>

int main()
{
    int fd;
    char buf[100];
    if((fd = open("b.txt", O_RDWR)) == -1)
    {
        printf("open failed\n");
        exit(1);
    }
    printf("%d\n", fd);
    //int a = dup(fd);
    //printf("a = %d\nfd = %d\n", a, fd);
    //close(0);
    dup2(fd,1);
    read(fd, buf, 100);
    printf("%s\n",buf);

    return 0;
}