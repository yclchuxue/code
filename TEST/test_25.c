#include<stdio.h>
#include<stdlib.h>
#define MAXVEX 20
typedef struct
{
	int data[MAXVEX];
	int top;
}SeqStack;

int visited[MAXVEX]={0}; 

typedef struct
{
	int arcs[MAXVEX][MAXVEX];
	char vex[MAXVEX];
	int vexnum;
	int arcnum; 
}AdjMatrix;

int LocateVex(AdjMatrix *G,char v)
{
	int i;
	for(i=1;i<=G->vexnum;i++)
	{
		if(G->vex[i]==v)return i;
	}
	return 0;
}

void Create(AdjMatrix *G)
{
	int i,j,k;
	char vex1,vex2;
	scanf("%d %d",&G->vexnum,&G->arcnum); 
	getchar();
	for(i=1;i<=G->vexnum;i++)
		for(j=1;j<=G->vexnum;j++)
			G->arcs[i][j]=0;
	for(i=1;i<=G->vexnum;i++)
	{
		scanf("%c",&G->vex[i]);
	}
	getchar();
	for(k=0;k<G->arcnum-1;k++)
	{
		scanf("<%c,%c>",&vex1,&vex2);
		i=LocateVex(G,vex1);
		j=LocateVex(G,vex2);
		G->arcs[i][j]=1;
	}
}

void ID(AdjMatrix g,int a[MAXVEX])
{
	int i,j;
	for(i=1;i<=g.vexnum;i++)
	{
		for(j=1;j<=g.vexnum;j++)
		{
			if(g.arcs[i][j]==1)
				a[j]++;
		}
		
	}
}
SeqStack *InitStack()
{
	SeqStack *s;
	s=(SeqStack *)malloc(sizeof(SeqStack));
	s->top=-1;
	return s; 
}
int Empty(SeqStack *s)
{
	if(s->top==-1)
		return 1;
	else return 0;
}
int Push(SeqStack *s,int x)
{
	if(s->top==MAXVEX-1)return 0;
	else{
		s->top++;
		s->data[s->top]=x;
		return 1;
	}
}
int Pop(SeqStack *s,int *x)
{
	if(Empty(s))return 0;
	else{
		*x=s->data[s->top];
		s->top--;
		return 1;
	}
}
int TopoSort(AdjMatrix g,int a[MAXVEX])
{
	int i,count,j;
	SeqStack *s = InitStack();
	for(i=1;i<=g.vexnum;i++)
		if(a[i]==0) Push(s,i);
	count=0;
	while(!Empty(s))
	{
		Pop(s,&i);
		printf("%c",g.vex[i]);
		count++;
		for(j=1;j<=g.vexnum;j++)
		{
			if(g.arcs[i][j]==1)
			{
				a[j]--;
				if(a[j]==0)
					Push(s,j);
			}
		}
	}
	if(count<g.vexnum)return 0;
	else return 1;
}

int main()
{
	AdjMatrix g;
	int s_ID[MAXVEX]={0};
	Create(&g);
	ID(g,s_ID);
	TopoSort(g,s_ID); 

	return 0;
}
