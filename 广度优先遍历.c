#include<stdio.h>
#include<math.h>
int a[401][401], book[401][401]; 
struct node
{
	int x;
	int y;
	int s;
}q[160002];

int main()
{
	int tail=1;
	int head=1;
	int m, n, startx, starty,tx,ty;
	scanf("%d%d%d%d", &m, &n, &startx, &starty);
	int next[8][2] = { {-2,1},{-1,2},{1,2},{2,1},{2,-1},{1,-2},{-1,-2},{-2,-1} };
	q[tail].x = startx;
	q[tail].y = starty;
	q[tail].s = 0;
	tail++;
	book[startx][starty] = 1;
	a[startx][starty] = 0;
	while (head < tail)
	{
		for (int k = 0; k < 8; k++)
		{
			tx = q[head].x + next[k][0];
			ty = q[head].y + next[k][1];
			if (tx<1 || tx>m || ty<1 || ty>n)
				continue;
			if (book[tx][ty] == 0)
			{
				book[tx][ty] = 1;
				q[tail].x = tx;
				q[tail].y = ty;
				q[tail].s = q[head].s + 1;
				a[tx][ty] = q[tail].s;
				tail++;
			}
		}
		head++;
	}
	for (int i = 1; i <=m; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			if (a[i][j] == 0 &&( i != startx || j != starty))
			{
				a[i][j] = -1;
			}
			printf("%-5d", a[i][j]);
		}
		printf("\n");
	}
	return 0;
}