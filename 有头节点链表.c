#include <stdio.h>
#include <stdlib.h>

typedef struct Listnode{
    int val;
    struct Listnode *next;
}Listnode;

void print(Listnode *list)                   //正向输出链表函数
{
    Listnode *p = list->next;
    for(;p;p = p->next)
    {
        printf("%d\t",p->val);
    }
}

void weicha(Listnode *list)                   //尾部插入函数
{
    printf("请输入你要插入到末尾的数：\n");
    int n;
    scanf("%d",&n);
    Listnode *p = list->next;
    while(p->next != NULL)
    {
        p = p->next;
    }
    Listnode *q = (Listnode*)malloc(sizeof(Listnode));
    q->val = n;
    q->next = NULL;
    p->next = q;
}

void shanchu(Listnode *list)                    //删除第k个节点函数
{
    printf("请输入要删除的节点：\n");
    Listnode *p = list->next;
    Listnode *q = list;
    int k,i = 1;
    scanf("%d", &k);
    while(i != k)
    {
        p = p->next;
        q = q->next;
        i++;
    }
    q->next = p->next;
}

int main()
{
    Listnode *list = (Listnode*)malloc(sizeof(Listnode));
    Listnode *temp = list;
    int n;
    do
    {
        scanf("%d",&n);
        if(n != -1)
        {
            Listnode *p = (Listnode*)malloc(sizeof(Listnode));
            p->val = n;
            p->next = NULL;
            temp->next = p;
            temp = p;
        }
    } while (n != -1);
    print(list);
    printf("\n");
    weicha(list);
    print(list);
    printf("\n");
    shanchu(list);
    print(list);

}