#include <stdio.h>
int main()
{
	int n, m = 0, h=0, t;
	scanf("%d",&n);
	if(n>=0)
	{
		h=1;
		t = n;
	}
	else
	{
		h=-1;
		t = -n;
	}
	while(t)
	{
		m = m*10 + t%10;
		t/=10;
	}
	printf("%d",m*h);
	return 0;
}