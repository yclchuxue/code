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

    scanf("%c", &ch);
    
    if(EOF)
    {
        printf("EOF!!!");
        exit(1);
    }

    printf("A%c\n", ch);

    return 0;
}