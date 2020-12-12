#include <stdio.h>
int main()
{
	int n, s[100], a[100];
	scanf("%d", &n);
	for (int i = 0; i < n; i++)
	{
		scanf("%d", &s[i]);
	}
	for (int i = 0; i < n; i++)
	{
		int t = 0;
		for (int j = 0; j < i; j++)
		{
			if (s[j] < s[i])
			{
				t++;
			}
		}
		a[i] = t;
	}
	for (int i = 0; i < n; i++)
	{
		printf("%d ", a[i]);
	}

	return 0;
}