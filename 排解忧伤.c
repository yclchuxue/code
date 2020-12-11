#include <stdio.h>
int s[1000000];
int main()
{
    int n,m;
    scanf("%d%d",&n,&m);
    for(int i = 0;i<m;i++)
    {
        scanf("%d",&s[i]);
    }
	int flag=0;
	int aa=0;
	long long ans=0;
	for(int i=1;i<=n;i++){
		if(s[i]!=0){
			aa+=s[i]-1;
			ans+=aa;
		}else{
			if(aa>0)
			{
				aa--;
				ans+=aa;
			}
		}
	}
	if(aa>0){
		printf("-1\n");
	}else{
		printf("%lld\n",ans);
	}

    return 0;
}