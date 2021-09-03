/******************************
 * 探究ctrl + D
 * ***************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    char strs[100], ch;
    fgets(strs, 100, stdin);

    printf("%s\n", strs);

    int n = scanf("%c", &ch);
    
    if(n == -1)
        printf("EOF\n");

    if(EOF)
    {
        printf("HJDBDJK\n");
    }

    printf("%d\n", n);
    printf("A%c\n", ch);

    return 0;
}