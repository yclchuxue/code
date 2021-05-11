#ifndef SHELL_HEAD_H
#define SHELL_HEAD_H
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
#include<stdbool.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>

// #include<history.h>
#define COLOR_BAGIN  "\001\033[1m\002"//强调，加粗，高亮
#define COLOR_END "\001\033[0m\002"
#define COLOR_BKG_SEF(x,y) "\001\033["#x";"#y"m\002"
#define COLOR_BKG(x) "\001\033["#x"m\002"//x<40-49>
#define COLOR_SEF(x) "\001\033["#x"m\002"//x<30-39>

#define DIRNAMESIZE 255
#define ARGMAXLEN 255
#define PIPE 1
#define INPUT 2
#define OUTPUT 4
#define APPPUT 8
#define BACKGROUND 16
#define EXIT 32
#define ARGMAXNUM 255
#define FILENAMESIZE 255
#define SHOWPATH 1
#define FILE1 "/tmp/file1"
#define FILE2 "/tmp/file2"
#define HISTORYFILE "historyfile"
#define HISTORYMAXSIZE 1000

struct history
{
    int id;
    char buf[ARGMAXLEN]; 
};

void get_prefix(char*str);
int  find_2(char*String,char*str);
int  get_mode(int*mode,char*input_str);
int back_check(char*str,int *mode);
char*s_gets(char*s,int n);
bool get_input(char*);
bool search_cmd(char*);
void execute_cmd(char**);
void print_shell();
void analysisArg(char**arg,int argcount,int *mode);
void sys_err(char*);
void split(char*subExpression,char**arglist,int*argcnt);
void split_2(char*String,char ch,char arglist[ARGMAXNUM][ARGMAXLEN],int *argNum);
int find(char*String,char c);
int do_arg(char*str,int mode);
void showdir(char*dirName);
void changdir(const char*newDir,char*oldDir);
char*format_blank(char*str);
void getUsername(char*username);       //获取usr名
void getHostname(char*hostname);       //获取用户名
int printHistory();
int getHistory(struct history *htylist,int *historylen);
int setHistory(const char*history);
void printHistory_2();
int setHistory_2(const char*input_str);

#endif

void sys_err(char*err)
{
    if(err!=NULL)
        fprintf(stderr,"%s\n",err);
    // exit(0);
}
char*s_gets(char*s,int n)
{   
    char*find;
    char*ret_val;
    if(n>ARGMAXLEN){return NULL;}
    ret_val=fgets(s,n,stdin);
    if(ret_val){
        find=strchr(s,'\n');
        if(find){
            *find='\0';
        }else{
            while(getchar()!='\n')
                continue;
        }
    }
    return ret_val;
}
void print_shell()
{
    char hname[BUFSIZ];
    char uname[BUFSIZ];
    char dirname[BUFSIZ];
    getHostname(hname);
    getUsername(uname);
    getcwd(dirname,BUFSIZ);  

    if(SHOWPATH){
        printf(COLOR_BAGIN COLOR_BKG_SEF(41,33) "%s@%s"COLOR_END":"COLOR_BAGIN COLOR_BKG_SEF(46,33) "%s$ "COLOR_END,hname,uname,dirname);//40 ,34
    }else{
        printf(COLOR_BAGIN COLOR_BKG_SEF(41,33) "%s@%s"COLOR_END"$ ",hname,uname);
    }
    return;
}
void get_prefix(char*str)
{
    char hname[BUFSIZ];
    char uname[BUFSIZ];
    char dirname[BUFSIZ];
    getHostname(hname);
    getUsername(uname);
    getcwd(dirname,BUFSIZ);  
    if(SHOWPATH){
        sprintf(str,COLOR_BAGIN COLOR_BKG_SEF(41,33) "%s@%s"COLOR_END":"COLOR_BAGIN COLOR_BKG_SEF(46,33) "%s$ "COLOR_END,hname,uname,dirname);
    }else{
        sprintf(str,COLOR_BAGIN COLOR_BKG_SEF(41,33) "%s@%s"COLOR_END"$ ",hname,uname);
    }
}

bool get_input(char*argline)
{
    if(s_gets(argline,ARGMAXLEN))
        return 1;
    else{
        sys_err("get_arg");
        return 0;   
    }
}

void split(char*subExpression,char**arglist,int*argcnt)
{
    char*cur=subExpression;//ls -l\0
    while(*cur==' '||*cur=='\t')
        cur++;
    char*begin=cur;
    int argNum=0;
    int len=0;
    while(cur&&*cur!='\0'){
        if(*cur==' '){
            arglist[argNum] = (char *)malloc(ARGMAXLEN*sizeof(char));
            memset(arglist[argNum],0,ARGMAXLEN);
            strncpy(arglist[argNum],begin,len);
            arglist[argNum][len]='\0';
            len=0;
            while(*cur==' '||*cur=='\t')
                cur++;
            begin=cur;
            argNum++;
        }else{
            len++;
            cur++;
        }
    }
    arglist[argNum]= (char *)malloc(ARGMAXLEN*sizeof(char));
    memset(arglist[argNum],0,ARGMAXLEN);
    strncpy(arglist[argNum],begin,len);
    arglist[argNum][len]='\0';
    arglist[++argNum]=NULL;
    *argcnt=argNum;
}
int back_check(char*str,int *mode)
{
    char*f=strchr(str,'&');
    char*p=f;
    if(p){
        p++;
        while(*p!='\0'){
            if(*p!=' '&&*p!='\t'){
                *mode&=~BACKGROUND;
                sys_err("& err");
                return -1;
            }
            p++;
        }
        *f='\0';
        *mode|=BACKGROUND;
        return 0;
    }else{
        return 0;
    }
}
bool search_cmd(char*cmd)
{
    DIR*dp;
    struct dirent*dirp;
    char newcmd[ARGMAXLEN];
    char*path[]={"./","/bin","/usr/bin",NULL,NULL};
    char*find=strrchr(cmd,'/');
    if(find){
        char newpath[ARGMAXLEN];
        strncpy(newpath,cmd,find-cmd+1);
        int i;
        for( i=0;path[i]!=NULL;i++){
        }
        path[i]=newpath;  
        strcpy(newcmd,find+1);
    }else{
        if(strncmp(cmd,"./",2)==0)
            cmd+=2;
        strcpy(newcmd,cmd);
    }
    // printf("n=%s\n",newcmd);
    for (int i = 0; path[i]!=NULL; i++)
    {
        if((dp=opendir(path[i]))==NULL)
            sys_err("cant open \n");
        while((dirp=readdir(dp))!=NULL){
            // printf("d=%s\n",dirp->d_name);
            if(strcmp(dirp->d_name,newcmd)==0){
                closedir(dp);
                return 1;
            }
        }
        closedir(dp);
    }
    return 0;
}
void split_2(char*String,char ch,char arglist[ARGMAXNUM][ARGMAXLEN],int *argNum)
{
    char*p=String;
    char*begin=p;
    *argNum=0;
    int len=0;
    while(*p!='\0'){
        if(*p==ch){
            
            if(len!=0){
                strncpy(arglist[(*argNum)],begin,len);
                arglist[(*argNum)++][len]='\0';
            }
            if(*argNum==ARGMAXNUM)
                sys_err("too many arg");
            p++;
            begin=p;
            len=0;
        }else{
            len++;
            p++;
        }
    }
    if(len!=0){
        strncpy(arglist[(*argNum)],begin,len);
        arglist[(*argNum)++][len]='\0';
    }
}
int  find(char*String,char c)
{
    int cnt=0;
    char*p=String;
    int len=0;
    while(p!=NULL&&*p!='\0'){
        if(*p==c&&(*(p+1)!=c)){
            if(len>=1&&*(p-1)==c){
                 p++;
                len++;
                continue;
            }
            if((p+1!=NULL)&&(*(p+1)==c)){
                p++;
                len++;
                continue;
            }
            cnt++;
        }
        p++;
        len++;
    }
    return cnt;
}
int  find_2(char*String,char*str)
{
    // llllllllll<< <<\0
    int cnt=0;
    char*p=String;
    int StrLen=strlen(String);
    int strLen=strlen(str);
    while(p&&*p!='\0'){
        if(strncmp(p,str,strLen)==0){
            cnt++;
        }
        p++;
    }
    return cnt;
}
int do_arg(char*str,int mode)
{
    // printf("%d\n",mode);
    char arglist2[ARGMAXNUM][ARGMAXLEN];
    int argNum2=0;
    
    char fileName[FILENAMESIZE];
    if(mode&INPUT){
        split_2(str,' ',arglist2,&argNum2);
        char*arg[argNum2+1];
        for(int j=0;j<argNum2;j++){
            arg[j]=(char*)arglist2[j];
        }
        arg[argNum2]=NULL;
        
        int i;
        for(i=0;i<argNum2;i++)
            if(strcmp(arg[i],"<")==0&&i==argNum2-2&&i!=0){
                strncpy(fileName,arglist2[i+1],FILENAMESIZE);
                arg[i]=NULL;
                break;
            }
        
        int fd=open(fileName,O_RDONLY);
        if(fd==-1){
        perror("open");
        // sys_err("open");
        return -2;
        }
        int pid=fork();
        if(pid==-1){
            sys_err("fork");
            return -3;
        }else if(pid==0){
            dup2(fd,0);
            if(search_cmd(arg[0])==false){
                sys_err("找不到文件");
                exit(0);
            }
            if(execvp(arg[0],arg)==-1){
                sys_err("执行失败！！！");
                exit(0);   
            }
            
        }else{
            if(mode&BACKGROUND)
                printf("pid[%d]正在后台运行\n",pid);
            else 
                waitpid(pid,NULL,0);
            return 0;
        }
            
    }else if(mode&OUTPUT||mode&APPPUT){
        split_2(str,' ',arglist2,&argNum2);
        char*arg[argNum2+1];
        for(int j=0;j<argNum2;j++){
            arg[j]=(char*)arglist2[j];
        }
        arg[argNum2]=NULL;
        
        int i;
        for(i=0;i<argNum2;i++)
            if((strcmp(arg[i],">")==0||strcmp(arg[i],">>")==0)&&i==argNum2-2&&i!=0){
                strncpy(fileName,arglist2[i+1],FILENAMESIZE);
                arg[i]=NULL;
                break;
            }
            int fd;
            if(mode&OUTPUT)
                fd=open(fileName,O_CREAT|O_TRUNC|O_RDWR,0644);
            else if(mode&APPPUT)
                fd=open(fileName,O_CREAT|O_APPEND|O_RDWR,0644);
            if(fd==-1){
                perror("open");
                // sys_err("open");
                return -2;
            }
            int pid=fork();
            if(pid==-1){
                sys_err("fork");
                return -3;
            }else if(pid==0){
                dup2(fd,1);
                if(search_cmd(arg[0])==false){
                    sys_err("找不到文件");
                    exit(0);
                }
                if(execvp(arg[0],arg)==-1){
                    sys_err("执行失败！！！");
                    exit(0);   
                }
            }else{
                if(mode&BACKGROUND)
                    printf("pid[%d]正在后台运行\n",pid);
                else 
                    waitpid(pid,NULL,0);
                return 0;
            }
        
    }else if((!(mode&PIPE))&&(!(mode&OUTPUT))&&(!(mode&INPUT))){
        split_2(str,' ',arglist2,&argNum2);
        char*arg[argNum2+1];
        for(int j=0;j<argNum2;j++){
            arg[j]=(char*)arglist2[j];
        }
        arg[argNum2]=NULL;
        int pid=fork();
            if(pid==-1){
                sys_err("fork");
                return -3;
            }else if(pid==0){
                if(search_cmd(arg[0])==false){
                    sys_err("找不到文件");
                     exit(0);
                }
                if(execvp(arg[0],arg)==-1){
                    sys_err("执行失败！！！");
                    exit(0);   
                }
            }else{
                if(mode&BACKGROUND)
                    printf("pid[%d]正在后台运行\n",pid);
                else 
                    waitpid(pid,NULL,0);
                return 0;
            }

    }else if(mode&PIPE){
        split_2(str,'|',arglist2,&argNum2);
        char arglist3[ARGMAXNUM][ARGMAXLEN];
        char  infile[FILENAMESIZE];
        char  outfile[FILENAMESIZE];
        for(int i=0;i<argNum2;i++){
            int argNum3=0;
            if(i%2){
                strcpy(infile,FILE1);
                strcpy(outfile,FILE2);
            }else{
                strcpy(infile,FILE2);
                strcpy(outfile,FILE1);
            }
            split_2(arglist2[i],' ',arglist3,&argNum3);
            char*arg[argNum3+1];
            for(int j=0;j<argNum3;j++){
                arg[j]=(char*)arglist3[j];
            }
            arg[argNum3]=NULL;
            int pid=fork();
            if(pid==-1){
                sys_err("fork");
                return -3;
            }else if(pid==0){
                if(search_cmd(arg[0])==false){
                    sys_err("找不到文件");
                     exit(0);
                }
                if(i!=0){
                    int fd=open(infile,O_RDONLY);
                    // printf("i=%d\tinfile:%s:",i,infile);
                    if(fd==-1){
                        perror("open");
                        // sys_err("open");
                        return -2;
                    }
                    dup2(fd,0);
                    // rmdir("/tmp/mystery love");
                }
                // printf("%d--argNUM3:\n",argNum2);
                if(i!=argNum2-1){
                    // printf("i=%d,outfile:%s:",i,outfile);
                    int fd2=open(outfile,O_CREAT|O_TRUNC|O_RDWR,0644);
                    if(fd2==-1){
                        sys_err("open");
                        return -2;
                    }
                    dup2(fd2,1);
                    }
                    if(execvp(arg[0],arg)==-1){
                        sys_err("执行失败！！！");
                        exit(0);   
                    }
                }else{
                    // dup2(fd,1);
                    if(mode&BACKGROUND)
                        printf("pid[%d]正在后台运行\n",pid);
                    else 
                        waitpid(pid,NULL,0);
                    // return 0;
                    remove(infile);
                }

        }
    }
    
}
int  get_mode(int*mode,char*input_str)
{
        int ret=back_check(input_str,mode);
        if(ret==-1){
            return -1;
        }
        if(find(input_str,'|')){
            if(find_2(input_str,">>")||find(input_str,'>')||find(input_str,'<')){
                sys_err("   |  下不支持>,<,>>");
                return -1;
            }else            
                *mode|=PIPE;

        }else{
            int ocnt=find(input_str,'>');
            int icnt=find(input_str,'<');
            int acnt=find_2(input_str,">>");
            // printf("%d%d%d\n",ocnt,icnt,acnt);
            if(ocnt>1){
                sys_err("只支持1个>");
                return -1;
            }
             if(icnt>1){
                sys_err("只支持1个<");
                return -1;
            }
             if(acnt>1){
                sys_err("只支持1个<<");
                return -1;
            }
            if((ocnt==icnt&&icnt==1)||(ocnt==acnt&&acnt==1)||(icnt==acnt&&acnt==1)){
                 sys_err("不可同时支持< > >>");
                return -1;
            }
            if(ocnt==1){
                *mode|=OUTPUT;
            }else if(icnt==1){
                *mode|=INPUT;
            }else if(acnt==1){
                *mode|=APPPUT;
            }            
        }
        return 0;
}
void changdir(const char*newDirName,char*oldDirName)
{
    char newDir[ARGMAXLEN];
    strcpy(newDir,newDirName);
    if(strcmp(newDir,"-")==0){
        if(*oldDirName=='\0'){
           sys_err("这是第一次跳转目录,因此无法跳转到上一次的目录"); 
        }else{
            strcpy(newDir,oldDirName);            
        }   
    }else if(strcmp(newDir,"~")==0){
        char uname[BUFSIZ];
        getUsername(uname);
        sprintf(newDir,"/home/%s",uname);
    }
    getcwd(oldDirName,ARGMAXLEN);            
    if(chdir(newDir)==-1){
        fprintf(stderr,"错误的目录名称:%s\n",newDir);
        return ;
    }  
}
void showdir(char*dirName)
{
  getcwd(dirName,DIRNAMESIZE);   
  printf("%s\n",dirName);
}
char*format_blank(char*str)
{
    if(str==NULL)return str;
    int len=strlen(str);
    char*p=str,*e=str+len;
    while(p&&(*p!='\0')){
        if(*p==' '||*p=='\t'){
            p++;
        }else{
            break;
        }
    }
    while(e&&*e!=*p){
        if(*e==' '||*e=='\t'){
            *e='\0';
            e--;
        }else{
            break;
        }
    }
    return p;
}
void getUsername(char*username)
{
    struct passwd* pwd = getpwuid(getuid());
    strcpy(username, pwd->pw_name);
}

void getHostname(char*hostname)
{ 
    gethostname(hostname, BUFSIZ);
}

int setHistory(const char*input_str)
{
    if(input_str==NULL||*input_str=='\0')
        return -1;
    int historylen=0;
    struct history htylist[HISTORYMAXSIZE];
    char history[ARGMAXLEN];
    strcpy(history,input_str);
    for(int i=0;i<HISTORYMAXSIZE;i++)
        memset(htylist+i,0,sizeof(struct history));
    if(getHistory(htylist,&historylen)!=-1){
        if(historylen==0){
            htylist[historylen].id=0;
        }
        else{
            htylist[historylen].id=htylist[historylen-1].id+1;
        }
        strncpy(htylist[historylen].buf,history,HISTORYMAXSIZE);
        int fd=open(HISTORYFILE,O_APPEND|O_RDWR);
        if(fd==-1){
            perror("open");
            return -1;
        }
        int wlen=write(fd,htylist+historylen,sizeof(struct history));
        if(wlen==-1)
            fprintf(stderr,"%s\n",strerror(errno));
        close(fd);
        return 0;
    }else{
        return -1;
    }
}
int getHistory(struct history *htylist,int *historylen)
{
    int fd=open(HISTORYFILE,O_CREAT|O_RDONLY,0644);
    if(fd==-1){
        perror("open");
        return -1;
    }
    else {
        int ret=0;
        int i=0;
        while(ret=read(fd,htylist+i,sizeof(struct history))){
            if(ret==-1){
                fprintf(stderr,"read err %s",strerror(errno));
                return -1;
            }if(ret==0){
                break;
            }
            i++;
            (*historylen)++;
            if(i==HISTORYMAXSIZE){
                for(int j=0;j<HISTORYMAXSIZE;j++)
                    memset(htylist+j,0,sizeof(struct history));
                i=0;
                *historylen=0;
            }
        }
    }
    close(fd);
    return 0;
}
int printHistory()
{
    struct history htylist[HISTORYMAXSIZE];
    int htylen=0;
    if(getHistory(htylist,&htylen)==-1){
        return -1;
    }
    for(int i=0;i<htylen;i++){
        printf("%d %s\n",htylist[i].id,htylist[i].buf);
    }
    return 0;
}
int setHistory_2(const char*input_str)
{
    if(input_str!=NULL&&*input_str!='\0'){
        add_history(input_str);
        write_history(NULL);
        return 0;
    }else{
        return -1;
    }
}
void printHistory_2()
{
    int i = 0;
    HIST_ENTRY ** his;
    his = history_list();
    while(his[i] != NULL)
    {
        printf("%d %s\n",i, his[i]->line);
        i++;
    }
}