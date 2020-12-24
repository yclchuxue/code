//无头节点单链表反转（递归）
#include <stdio.h>
#include <stdlib.h>

typedef struct Listnode{
    int val;
    struct Listnode *next;
}Listnode;

void print(Listnode *head)
{
    Listnode *p = head;
    for(;p;p = p->next)
    {
        printf("%d\t",p->val);
    }
}

Listnode *dg(Listnode *qian,Listnode *hou)
{
    Listnode *p1 = qian;
    Listnode *p2 = hou;
    if(p1 != NULL)
    {
        Listnode *q = p1->next;
        p1->next = p2;
        dg(q,p1);
    }
    if(p1 == NULL)
    {
        return p2;
    }
}

Listnode *fz(Listnode *head)
{
    Listnode *p = head;
    Listnode *b = NULL;
    if(p->next == NULL)
    {
        b = head;
    }
    else
    {
        p = p->next;
        head->next = b;
        b = dg(p,head);
    }

    return b;
}

int main()
{
    int n;
    Listnode *head = NULL;
    Listnode *b = NULL;
    do
    {
        scanf("%d", &n);
        if(n != -1)
        {
            Listnode *p = (Listnode*)malloc(sizeof(Listnode));
            p->val = n;
            p->next = NULL;
            if(!head)
            {
                head = p;
                b = p;
            }
            else
            {
                b->next = p;
                b = b->next;
            }
        }
    }while(n != -1);
    print(head);
    printf("\n");
    Listnode *s = fz(head);
    print(s);

    return 0;
}