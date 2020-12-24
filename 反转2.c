//无头节点单链表反转（循环）
#include <stdio.h>
#include <stdlib.h>

typedef struct Listnode{
    int val;
    struct Listnode *next;
}Listnode;

void print(Listnode *head)
{
    Listnode *q = head;
    for(;q;q = q->next)
    {
        printf("%d\t", q->val);
    }
}

Listnode *fz(Listnode *head)
{
    Listnode* pre = NULL; 
    while (head != NULL) 
    { 
        Listnode* next = head->next; 
        head->next = pre; 
        pre = head; 
        head = next; 
    } 
    return pre;
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
    Listnode *p = fz(head);
    printf("\n");
    print(p);

    return 0;
}