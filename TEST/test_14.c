/****************************
 * 计算表达式结果--PTA
 * *************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int Is_ch(char ch)
{
    if(ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int ch1_less_ch2(char ch1, char ch2)
{
    if((ch1 == '+' || ch1 == '-') && (ch2 == '*' || ch2 == '/'))
    {
        return 1;
    }
    else if((ch1 == '+' || ch1 == '-') && (ch2 == '+' || ch2 == '-'))
    {
        return 1;
    }
    else if((ch1 == '/' || ch1 == '*') && (ch2 == '*' || ch2 == '/'))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int run_ab(int a, int b, char ch)
{
    if(ch == '+') return a+b;
    if(ch == '-') return a-b;
    if(ch == '*') return a*b;
    if(ch == '/') return a/b;

    return 0;
}

int main()
{
    char strs[100];
    char A[50];
    char B[50];
    char ch;
    //scanf("%s", &strs);
    int h = 0;
    while(1)
    {
        scanf("%c", &ch);
        if(ch == '#')
        {
            break;
        }
        strs[h] = ch;
        ++h;
    }
    
    int j = -1, k = -1,num = 0;
    //printf("%d", h);
    for(int i = 0; i < h; ++i)
    {
        if(!Is_ch(strs[i]))        //数字
        {
            num = num*10 + (strs[i] - '0');
            //++j;
            //A[j] = strs[i];
            
            //printf("%c", strs[i]);
        }
        else                       //符号
        { 
            ++j;
            A[j] = num;
            num = 0;
            if(k == -1)
            {
                ++k;
                B[k] = strs[i]; 
            }
            else
            {
                if(strs[i] == ')')
                {
                    while(1)
                    {
                        if(B[k] != '(')
                        {
                            //printf("%c", B[k]);
                            A[j-1] = run_ab(A[j-1], A[j], B[k]);
                            --j;
                            --k;
                        }
                        else
                        {   
                            --k;
                            break;
                        }
                    }
                }
                else
                {
                    while(1)
                    {
                        if(strs[i] != '(' && ch1_less_ch2(strs[i], B[k]))   //满足优先级小或）
                        {   
                            //printf("%c", B[k]);
                            //printf("%d\t'%c'\t%d\n", A[j-1],B[k],A[j]);
                            A[j-1] = run_ab(A[j-1], A[j], B[k]);
                            --j;
                            //printf("ret1 = %d\n", A[j]);
                            --k;
                        }
                        else       //优先级大
                        {
                            ++k;
                            B[k] = strs[i];
                            break;
                        }
                    }
                }
            }
        }
    }
    ++j;
    A[j] = num;
    if(k != -1)
    {
        for(int i = k; i >= 0; --i)
        {
            A[j-1] = run_ab(A[j-1], A[j], B[i]);
            --j;
            //printf("ret2 = %d\n", A[j]);
            //printf("%c", B[i]);
        }
    }
    printf("%d", A[0]);

    return 0;
}