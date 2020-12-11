#include <stdio.h>

int min(int m,int n)
{
    if(m<n)
    {
        return m;
    }
    else
    {
        return n;
    }
}

double long pai = 3.1415926535;
int main()
{
    int t;
    scanf("%d", &t);
	while(t--)
    {
		double a,b,r,h;
		scanf("%lf%lf%lf%lf",&a,&b,&r,&h);
		double e=min(abs(a-b),360-abs(a-b));
		double ans=(e*2*pai*r/360.0)*(e*2*pai*r/360.0)+h*h;
		printf("%.2lf\n",ans);
	}
}