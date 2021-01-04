#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int val;
    struct node *next;
}Listnode;

void print(Listnode *head)
{
    Listnode *p = head;
    for(;p;p=p->next)
    {
        printf("%d\t", p->val);
    }
}

int main()
{
    int n,k,m = 0;
    scanf("%d",&k);
    Listnode *head = NULL;
    Listnode *tail = NULL;
    while(m<k)
    {
        scanf("%d",&n);
        Listnode *p = (Listnode*)malloc(sizeof(Listnode));
        p->val = n;
        p->next = NULL;
        if(head == NULL)
        {
            head = p;
            tail = p;
        }
        else
        {
            tail->next = p;
            tail = tail->next;
        }
        m++;
    }
    print(head);
    Listnode *p = head;
    Listnode *zhong = NULL;
    for(int i = 0; i <(k+1)/2;i++)
    {
        zhong = head;
        head = head->next;
    }
    printf("\n%d\n",zhong->val);

    return 0;
}