#include <stdio.h>
#include <math.h>
/*
//投机取巧
int mySqrt0(int x)            
{
    int m = pow(x,1/2.0);

    return m;
}

//暴力
int mySqrt1(int x)
{
    if(x == 1)
    {
        return 1;
    }
    for(long i = 0; i <= x/2;i++)
    {
        if(i*i == x)
        {
            return i;
        }
        if(i*i < x && (i+1)*(i+1) > x)
        {
            return i;
        }
    }
    return 0;    
}
*/

//二分法
int mySqrt2(int x)
{
    long left = 0,right = x,mid;
    while(left <= right)
    {
        mid = (left + right)/2;
        if(mid*mid > x)
        {
            right = mid-1;
        }
        else if(mid*mid < x)
        {
            left = mid + 1;
        }
        else if(mid*mid == x)
        {
            return mid;
        }
    }
    return left-1;
}

int main()
{
    int x;
    scanf("%d",&x);
    int n = mySqrt2(x);
    printf("%d",n);

    return 0;
}