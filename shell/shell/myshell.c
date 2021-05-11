#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include<sys/mman.h>
#include<pthread.h>
#include<semaphore.h>
#include <netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/select.h>
#include<sys/epoll.h>
#include<unistd.h>
#include<dirent.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<errno.h>
#include<signal.h>
#include<fcntl.h>
#include"shell_head.h"

int main(int argc,char**argv)
{
    read_history(NULL);
    signal(SIGINT,SIG_IGN);
    char oldDir[ARGMAXLEN];
    memset(oldDir,0,ARGMAXLEN);
    // char input_str[ARGMAXLEN];
    while(1){
        // print_shell();
        char prefix_str[ARGMAXLEN];
        char input_str[ARGMAXLEN];
        get_prefix(prefix_str);
        char*buf=readline(prefix_str);//COLOR_BAGIN COLOR_BKG_SEF(49,34)" " COLOR_END
        // get_input(input_str);
        strncpy(input_str,buf,ARGMAXLEN);
        free(buf);
        if(setHistory_2(input_str)==-1){
            continue;
        }
        if(strcmp(input_str,"exit")==0||strcmp(input_str,"logout")==0){
            exit(0);
        }
        else if(strncmp(input_str,"cd",2)==0&&((input_str[2]=='\0')||(input_str[2]==' '))){
            char*s=format_blank(input_str+2);
            changdir(s,oldDir);
            continue;
        }else if(strncmp(input_str,"history",7)==0&&((input_str[7]=='\0')||(input_str[7]==' '))){
            printHistory_2();
            continue;
        }
        int mode=0;
        if(get_mode(&mode,input_str)!=0){
            continue;
        }
        int ret2=do_arg(input_str,mode);
        if(ret2!=0){
            continue;
        }        
    }
    return 0;
}