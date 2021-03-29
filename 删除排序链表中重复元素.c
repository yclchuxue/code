#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ListNode {
    int val;
    struct ListNode *next;
};



struct ListNode* deleteDuplicates(struct ListNode* head)
{
    struct ListNode *h = (struct ListNode*)malloc(sizeof(struct ListNode));
    h->next = head;
    struct ListNode *p = head;
    struct ListNode *q = p->next;
    printf("A");
    for(;q;q = q->next)
    {
        while(p->val == q->val && q)
        {
            q = q->next;
            p = p->next;
        }
        h->next = q;
        p = q;
        q = q->next;
    }

    return h->next;
}

int main()
{
    struct ListNode *head = NULL;
    struct ListNode *tail = NULL;
    int val;
    scanf("%d",&val);
    while(val != -1)
    {
        struct ListNode *p = (struct ListNode*)malloc(sizeof(struct ListNode));
        p->val = val;
        if(head == NULL)
        {
            head = p;
            tail = p;
        }
        tail->next = p;
        tail = p;
        scanf("%d",&val);
    }
    printf("A");
    struct ListNode *x = deleteDuplicates(head);
    struct ListNode *q = x;
    for(;q;q = q->next)
    {
        printf("%d   ",q->val);
    }

    return 0;
}