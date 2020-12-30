#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Tree
{
    int count;
    struct Tree *child[26];
}Node,*Trie_node;

Node *CreateTrie()
{
    Node *node = (Node*)malloc(sizeof(Node));
    memset(node, 0, sizeof(Node));
    return node;
}

void insert_node(Trie_node root,char *str)
{
    if(root == NULL||*str == '\0')
        return;
    Node *t = root;
    char *p = str;
    while(*p!='\0')
    {
        if(t->child[*p-'a']==NULL)
        {
            Node *tmp = CreateTrie();
            t->child[*p-'a'] = tmp;
        }
        t = t->child[*p-'a'];
        p++;
    }
    t->count++;
}

void search_str(Trie_node root,char *str)
{
    if(NULL==root || *str=='\0')
    {
        printf("trie is empty or str is null\n");
        return;
    }
    Node *t = root;
    char *p = str;
    while(*p!='\0')
    {
        if(t->child[*p-'a'] != NULL)
        {
            t = t->child[*p-'a'];
            p++;
        }
        else
        {
            break;
        }
    }
    if(*p == '\0')
    {
        if(t->count == 0)
        {
            printf("该字符串不在trie树中，但该串是某个单词的前缀\n");
        }
        else
        {
            printf("该字符串在该trie树中\n");
        }
    }
    else
    {
        printf("该字符串不在trie树中\n");
    }
}

void del(Trie_node root)      //释放整个字典树占的堆空间
{
    int i;
    for(i=0;i<26;i++)
    {
        if(root->child[i]!=NULL)
            del(root->child[i]);
    }
    free(root);
}


int main()
{
    int i, n, h;
    char str[20];
    printf("请输入要创建数的大小：\n");
    scanf("%d",&n);
    Trie_node root=NULL;
    root=CreateTrie(); 
    if(root==NULL)
        printf("创建trie树失败\n");
    for(i=0;i<n;i++) 
    {
        scanf("%s",str);
        insert_node(root,str);
    }
    printf("tree建立完成\n");
    printf("请输入要查询的单词的个数：\n");
    scanf("%d",&h);
    i = 0;
    while(i<h)
    {
        printf("请输入要查询的单词：\n");
        scanf("%s",str);
        search_str(root,str);
        i++;
    }
    del(root);

    return 0;
}