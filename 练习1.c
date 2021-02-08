#include<stdio.h>
#include<string.h>
int main()
{
	int m=0,j,i,t1;
	char a[15];
	scanf("%s",a);
	m=strlen(a);
	for(i=0;i<m-1;i++)
		{
			for(j=0;j<m-i-1;j++)
			{
				if(a[j]>a[j+1])
				    {
					    t1=a[j];
				    	a[j]=a[j+1];
				     	a[j+1]=t1;
			    	}
			}
				
		}
		for(i=0;i<=m;i++)
		{
			printf("%c",a[i]);
		}
		return 0;
}