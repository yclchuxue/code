#include <stdio.h>
#include <string.h>

void git_next(char *a, int *b,int m)
{
    b[0] = 0;
    for(int i = 1; i < m; i++)
    {
        int h = 0;
        for(int j = 1;j<=i;j++)
        {
            if(a[i-j] == a[m-1-j])
            {
                h++;
            }
        }
        if(h == i)
        {
            b[i] = h;
        }
        else
        {
            b[i] = 0;
        }
        
    }
}

int KMP(char s[],char h[],int b[],int s_len,int h_len)
{
    int i,j;
    for(i = 0, j = 0; i < s_len&& j <h_len; i++,j++)
    {
        if(s[i] != h[j])
        {
            int max = 0;
            for(int k = 0;k<j;k++)
            {
                if(b[max]<b[k])
                {
                    max = k;
                }
            }
            j = max;
        }
    }
    return i-h_len;
}



int main()
{
    char s[100],h[100];
    fgets(s,100,stdin);
    fgets(h,100,stdin);
    int n = strlen(s)-1;
    int m = strlen(h)-1;
    printf("%d\n",m);
    int b[100];
    git_next(h,b,m);
    int k = KMP(s,h,b,n,m);
    printf("\n%d",k);
    return 0;
}