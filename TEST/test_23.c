//DPS
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#define MAXVEX 20
#define INFINITY 10000

typedef struct Arcnode
{
	int arcnode[MAXVEX][MAXVEX];
	int vexnum1;	
	int arcnum2;	
	char vex[MAXVEX];
    int visited[MAXVEX];
}AdjMatrix;

typedef struct Queue 
{
		char Array[MAXVEX];
		int rear;
		int front;
}Queue;

int Locate(AdjMatrix *G,char v)
{
	int i;
	for(i=1;i<=G->vexnum1;i++)
	{
		if(G->vex[i] == v)
		{
			return i;
		}
			
	}	
		return 0;
}

void Create(AdjMatrix *G)
{
	int i,j,k,vex1,vex2;
	char Vex1,Vex2;
 
	scanf("%d %d",&G->vexnum1,&G->arcnum2);
	for(i = 1;i<=G->vexnum1;i++){
		for(j = 1;j<=G->vexnum1;j++){
			G->arcnode[i][j] = 0;
		}
	}
	getchar();
 
	for(i = 1;i<=G->vexnum1;i++){
 
		scanf("%c",&G->vex[i]);
	}
	getchar();
	for(k = 0;k<G->arcnum2;k++){
 
		scanf("%c",&Vex1);
		vex1 = Vex1-64;
 
		scanf("%c",&Vex2);
		vex2 = Vex2-64;
		getchar();
//		printf("%d %d\n",vex1,vex2);
		G->arcnode[vex1][vex2] = 1;
	}
 } 
 void initialise(AdjMatrix *G)
 {
 	int i;
	for(i=0;i<=G->vexnum1;i++)
	{
	 	G->visited[i]=0;
	}	
 }
 void DFS(AdjMatrix *G,int Vo)
 {
	int i;	
	//printf("%c",G->vex[Vo]);
	G->visited[Vo] = 1;    
	
	for(i = 1;i<=G->vexnum1;i++)
	{
		if(!G->visited[i]&&G->arcnode[Vo][i]==1)
		{
			DFS(G,i);
		} 
	} 
}
int FirstAdj(AdjMatrix *G,int Vo)
{
	for(int i = 1;i<=G->vexnum1;i++){
		if(G->arcnode[Vo][i]==1&&G->visited[i]!=1){
			return i;
		}
	}
	return -1;
}
int NextAdj(AdjMatrix *G,int Vo,int temp)
{
	for(int i = temp+1;i<=G->vexnum1;i++)
	{
		if(G->arcnode[Vo][i]==1&&G->visited[i]!=1)
		{
			return i;
		}
	}
	return -1;
}
 
void BFS(AdjMatrix *G,int Vo)
{
 
	//printf("%c\n",G->vex[Vo]);
	G->visited[Vo]= 1;
	Queue *Q = (Queue *)malloc(sizeof(Queue));
	Q->front = 0;
	Q->rear = 0;
	Q->front++;
	Q->Array[Q->front] =Vo;
	
	while(Q->front-Q->rear!=0)
	{
		Vo = Q->Array[Q->front];
		Q->rear++;
		int Temp;
		Temp = FirstAdj(G,Vo);
		//printf("%d",Temp);
		
		while(Temp!=-1)
		{
			if(!G->visited[Temp])
			{
				//printf("%c",G->vex[Temp]);
				G->visited[Temp]=1;
				Q->front++;
				Q->Array[Q->front] = Temp;
			}
			Temp = NextAdj(G,Vo,Temp);
			//printf("%d",Temp);
		}
	}
}
 
int visited[MAXVEX] = {0};    //访问标志数组 
void TraverseG(AdjMatrix *G)
{
	for(int v = 0;v<G->vexnum1;v++)
		visited[v] = 0;
		for(int v = 0;v<G->vexnum1;v++)
			if(!visited[v])
				BFS(G,v);
}
 
int main(void)
{
	int i,j;
	int count = 0;
	AdjMatrix *G = (AdjMatrix *)malloc(sizeof(AdjMatrix));
	Create(G);
	initialise(G);
	for(i = 1;i<=G->vexnum1;i++)
	{
		if(G->visited[i]!=1){
			BFS(G,i);
			count++;
		}
	}
	printf("%d",count);
	return 0;
}