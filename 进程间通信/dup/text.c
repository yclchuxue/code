#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int n;
    char buf[1024];

    while(1)
    {
        if((n = read(1, buf, 1024)) > 0)
        {
            buf[n] = '\0';
            printf("text receive: %s\n", buf);

            if(!strcmp(buf,"getpid"))
            {
                exit(0);
            }
            if(!strcmp(buf,"getpid"))
            {
                printf("My pid: %d\n", getpid());
                sleep(3);
                exit(0);
            }
        }
    }
}
