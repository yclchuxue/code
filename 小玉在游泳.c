#include <stdio.h>
int main()
{
	int i=0;
	double n = 2.0, k;
	double S=0;
	scanf("%lf",&k);
	while(S<k)
	{
		i++;
		S = S +n;
		n=n*98/100;
	}
	printf("%d",i);
	return 0;
}