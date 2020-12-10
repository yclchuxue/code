#include <stdio.h>

int pd(int y) 
{
	for(int i=2; i*i<=y; ++i) 
    {
		if(y%i==0) return 0;
	}
	return 1;
}

int main()
{
    int s[1000] = {0};
    int L,sum = 0, j, n = 0;
    scanf("%d",&L);
    for(int i = 2,j = 0;sum <= L;i++)
    {
        for(;pd(i) == 1;j++)
        {
            s[j] = i;
            n++;
            sum = sum +i;
        }
    }
    for(int i = 0;i<n;i++)
    {
        printf("%d\n",s[i]);
    }
    printf("%d",n);

    return 0;
}