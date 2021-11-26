#include <unistd.h> 
#include <stdio.h> 
#include <termios.h> 
#include <signal.h> 

void sig_hnd(int sig)
{ 
    (void)sig; 
    printf("(VINTR)"); 
}

int main()
{
    setvbuf(stdout,NULL,_IONBF,0);

    struct termios old_termios, new_termios;
    tcgetattr(0,&old_termios);                      //cgetattr函数用于获取与终端相关的参数

    //signal( SIGINT, sig_hnd );

    new_termios             = old_termios;
    new_termios.c_cc[VEOF]  = 3; // ^C
    new_termios.c_cc[VINTR] = 4; // ^D
    tcsetattr(0,TCSANOW,&new_termios);              //修改终端设置
    
    char A[100];
    while(1)
    {
        scanf("%s", A);
        printf("A: %s\n", A);
    }

    tcsetattr(0,TCSANOW,&old_termios);              //恢复终端设置
}