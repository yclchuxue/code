#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char ** buildArray(int* target, int targetSize, int n, int* returnSize)
{
    char **ret = malloc(sizeof(char*)*n*2);
    for(int j = 0,i = 1;i<=n&&j<targetSize;i++)
    {
        ret[*returnSize++] = "push";
        if(i<target[j])
        {
            ret[*returnSize++] = "pop";
        }
        else
        {
            j++;
        }
    }
    return ret;
}

int main()
{
    int A[100] = {0};
    int m,n,h = 0;
    printf("请输入数组大小：");
    scanf("%d %d",&m,&n);
    printf("请输入数组：\n");
    for(int i=0; i<m; i++)
    {
        scanf("%d",&A[i]);
    }
    char **p = buildArray(A,m,n,&h);
    for(int i = 0;i < h;i++)
    {
        printf("%s\n",&p[i]);
    }

    return 0;
}