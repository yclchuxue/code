//无权无向图的最短路径
#include <stdio.h>
#include <stdlib.h>

#define Max 10000
#define Inf 1E5

typedef struct GNode* Graph;

struct GNode{
	int V;
	int L;
	int GR[Max][Max];
};
 
Graph CreateGraph(int V);
Graph InsertEdge(Graph G,int N);

void Cc(Graph G,int n);
int main(){
	int N,M,K,i,num;
	scanf("%d",&N);
	Graph G=CreateGraph(N);
	scanf("%d",&M);
	G=InsertEdge(G,M);
	scanf("%d",&K);
	for(i=0;i<K;i++){
		scanf("%d",&num);
		Cc(G,num);
	}
	return 0;	
}
Graph CreateGraph(int V){
	Graph G=(Graph)malloc(sizeof(struct GNode));
	G->V=V;
	int i,j;
	for(i=0;i<V;i++)
		for(j=0;j<V;j++)
		G->GR[i][j]=0;
	return G;
	
}
Graph InsertEdge(Graph G,int N){
	int i,v1,v2,w;
	for(i=0;i<N;i++){	 	
		scanf("%d %d",&v1,&v2);
		G->GR[v1-1][v2-1]=1;
		G->GR[v2-1][v1-1]=1;	
	}
	
	return G;	
}
void Cc(Graph G,int n){
	n--;
	int i, path=0, dis[Max], a[Max], k, head=0, tail=0; 
	for(i=0;i<G->V;i++){
		dis[i]=Inf;	
	}
	a[head]=n;
	dis[n]=0;
	while(head<=tail){
		k=a[head];
		head++;
		for(i=0;i<G->V;i++){
			if(G->GR[k][i]&&dis[i]>dis[k]+1){
				dis[i]=dis[k]+1;
				tail++;
				a[tail]=i;
			}		
		}		
	}
	for(i=0;i<G->V;i++){
		if(dis[i]!=Inf)
			path+=dis[i];
		else{
			path=Inf;
			break;
		}
	}
	if(path!=Inf){
		printf("Cc(%d)=%.2f\n",n+1,(G->V-1)*1.0/path);
	}
	else
		printf("Cc(%d)=0.00\n",n+1);
}