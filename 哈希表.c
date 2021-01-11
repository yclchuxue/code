#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct listnode{
    int val;
    char *name;
    struct listnode *next;
}Listnode;

void print(Listnode *head)
{
    Listnode *p = head;
    for(;p;p = p->next)
    {
        printf("%d\t%s\n",p->val,p->name);
    }
}

Listnode **hash(int n)
{
    Listnode **hashtable = (Listnode **)malloc(sizeof(*hashtable)*n);
    memset(hashtable, 0, sizeof(Listnode));
    return hashtable;
}

int key(int val)
{
    int key = 0;
    while(val != 0)
    {
        key = key + val%10;
        val = val/10;
    }
    return key;
}

void creat(Listnode *head, Listnode **hash,int n)
{
    Listnode *tail = NULL;
    for(int i = 1;i<=n;i++)
    {
        char *na = NULL;
        int va;
        int index = key(va);
        printf("请输入身份证号：");
        scanf("%d",&va);
        printf("\n请输入姓名：");
        scanf("%s",na);
        Listnode *p = (Listnode*)malloc(sizeof(Listnode));
        hash[index] = p;
        p->val = va;
        p->name = na;
        p->next = NULL;
        if(tail == NULL)
        {
            head = p;
            tail = p;
        }
        else
        {
            tail->next = p;
        }
        tail = p;
    }

}

int main()
{
    int n;
    printf("请输入要存放人员信息个数：\n");
    scanf("%d",&n);
    Listnode *head = NULL;
    Listnode **ha = hash(n);
    creat(head,ha,n);
    printf("\n请输入身份证号码：");
    int m;
    scanf("%d",&m);
    int index = key(m);
    printf("\n姓名：%s",ha[index]->name);
    print(head);
    return 0;
}