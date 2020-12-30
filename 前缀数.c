#include<stdio.h>
#include <stdlib.h>
#include<string.h>


#define MAX_CHILD 26 //此函数只考虑26个英文字母的情况




typedef struct Tree
{
    int count;         //用来标记该节点是个可以形成一个单词，如果count！=0，则从根节点到该节点的路径可以形成一个单词
    struct Tree *child[MAX_CHILD];
}Node,*Trie_node;




Node* CreateTrie()                             //创建trie节点树
{
    Node *node=(Node*)malloc(sizeof(Node));
    memset(node,0,sizeof(Node));
    return node;
}


void insert_node(Trie_node root,char *str)      //trie树插入结点
{
    if(root ==NULL || *str=='\0')
        return;
    Node *t=root; 
    char *p=str;
    while(*p!='\0')
    {
     if(t->child[*p-'a']==NULL)
        {
         Node *tmp=CreateTrie();
         t->child[*p-'a']=tmp;        
        }
     t=t->child[*p-'a'];
     p++;
    }
    t->count++;
}




void search_str(Trie_node root,char *str)             //查找串是否在该trie树中
{
    if(NULL==root || *str=='\0')
    {
     printf("trie is empty or str is null\n");
     return;
    }


    char *p=str;
    Node *t=root;
    while(*p!='\0')
    {     
     if(t->child[*p-'a']!=NULL)
        {
         t=t->child[*p-'a'];
            p++;
        }
     else
             break;
    }
    if(*p=='\0')
    {
     if(t->count==0)
            printf("该字符串不在trie树中，但该串是某个单词的前缀\n");
        else
            printf("该字符串在该trie树中\n");
    }
    else
printf("该字符串不在trie树中\n");
}


void del(Trie_node root)      //释放整个字典树占的堆空间
{
    int i;
    for(i=0;i<MAX_CHILD;i++)
    {
     if(root->child[i]!=NULL)
            del(root->child[i]);
    }
    free(root);
}




int main()
{
    int i,n;
    char str[20];
    printf("请输入要创建的trie树的大小：");
    scanf("%d", &n); 
    Trie_node root=NULL;
    root=CreateTrie(); 
    if(root==NULL)
        printf("创建trie树失败\n");
    for(i=0;i<n;i++) 
    {
        scanf("%s",str);
        insert_node(root,str);
    }
    printf("trie树已建立完成\n");
    printf("请输入要查询的字符串:\n");
    while(scanf("%s",str))
    {
     search_str(root,str);
    }
    return 0;
}