#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    char *Buf;//= (char *)malloc(sizeof(char)),
    char ch;
    int i = 0;
    
    do
	{
	    scanf("%c", &ch);
		if(ch != '\n')
		{
			Buf[i] = ch;
			i++;
		}
	}while(ch != '\n');
    Buf[i] = '\0';

    if(strlen(Buf) > 200)
    {
        printf("超出长度\n");
        Buf[199] = '\0';
    }

    printf("%s\n", Buf);

    printf("size = %d\n", strlen(Buf));

    return 0;
}