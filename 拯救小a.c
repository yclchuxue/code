#include <stdio.h>
char s[10000];
int main()
{
    char ch;
    int i = 0,sum = 0;
    do
    {
        scanf("%c", &ch);
        if(ch != '.')
        {
            s[i] = ch;
        }
        i++;
    } while (ch != '.');
    for(int j = 0;j<i;j++)
    {
        if(s[j] == 'a')
        {
            sum++;
        }
    }
    printf("%d",sum);

    return 0;
}