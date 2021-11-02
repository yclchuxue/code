#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int value;
    struct node *next;
}Node;

void get_list(Node *head, int *a, int len)
{
    Node *p = head;
    for(int i = 0; i < len; ++i)
    {
        Node *q = (Node *)malloc(sizeof(Node));
        q->value = a[i];

        p->next = q;
        q->next = NULL; 
        p = p->next;
    }

}

int main()
{
    int a[10] = {1,2,3,3,5,6,7,8,9,10};

    Node *head = (Node *)malloc(sizeof(Node));
    head->next = NULL;

    get_list(head, a, 10);
    
    int n, index = 0;
    scanf("%d", &n);

    Node *p = head;
    Node *q = head->next;
    head = head->next;
    while(head)
    {
        if(head->value == n)
        {
            index = 1;
            head = head->next;
            free(p->next);
            p->next = head;
        }
        else
        {
            p = p->next;
            head = head->next;
        }
    }

    if(index == 1)
    {
        while(q)
        {
            printf("%d\t", q->value);
            q = q->next;
        }
        printf("\n");
    }
    else
    {
        printf("NO\n");
    }

    return 0;
}