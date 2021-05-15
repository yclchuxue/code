#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <syslog.h>

int init_daemon(void)
{
    int pid;
    int i;

    /*忽略终端I/O信号，STOP信号*/
    signal(SIGTTOU,SIG_IGU);

}