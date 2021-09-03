#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <termios.h> 
#include <signal.h> 

int main()
{
    setvbuf(stdout,NULL,_IONBF,0);

    char line[256]; int len;
    do
    {
        len=read(0,line,20); 
        line[len]='\0';
        if( len <0 ) printf("(len: %i)",len);
        if( len==0 ) exit(1);
        if( len >0 )
        {
            if( line[len-1] == 10 ) printf("(line:'%.*s')\n",len-1,line);    //未超出界限，直接输出
            if( line[len-1] != 10 ) printf("(partial line:'%s')\n",line);    //超出界限，分段输出
        }
    }while( line[0] != 'q' );         

    return 0;
}