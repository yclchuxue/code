#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

//1.
struct ListNode* partition(struct ListNode* head, int x)
{
    struct ListNode*p11=NULL;
    struct ListNode*p22=NULL;
    struct ListNode*p = head;
    struct ListNode*last1,*last2;
    for(;p;p=p->next)
    {
        if(p->val<x)
        {
            struct ListNode*p1=(struct ListNode*)malloc(sizeof(struct ListNode));
            p1->val=p->val;
            p1->next=NULL;
            if(p11==NULL)
            {
                p11=p1;
                last1 = p1;
            }
            else
            {
                p11->next=p1;
            }
        }
        else
        {
            struct ListNode*p2=(struct ListNode*)malloc(sizeof(struct ListNode));
            p2->val=p->val;
            p2->next=NULL;
            if(p22==NULL)
            {
                p22=p2;
                last2 = p2;
            }
            else
            {
                p22->next=p2;
            }
        } 
    }
    p11->next=last2;

    for(;last1;last1=last1->next)
    {
        printf("%d\t",last1->val);
    }

    return 0;
}

//2.
struct ListNode* partition(struct ListNode* head, int x)
{ 
    struct ListNode*p1Head=(struct ListNode*)malloc(sizeof(struct ListNode));
    struct ListNode*p2Head=(struct ListNode*)malloc(sizeof(struct ListNode));
    struct ListNode*p1=p1Head; 
    struct ListNode*p2=p2Head; 
    struct ListNode*p=head; 
    while(p)
    { 
        if(p->val<x)
        { 
            p1->next=p; 
            p1=p; 
        } 
        else
        { 
            p2->next=p; 
            p2=p; 
        } 
        p=p->next; 
    } 
        p2->next=NULL; 
        p1->next=p2Head->next;
    return p1Head->next; 
}

int main()
{
    struct ListNode*head,*last=NULL;
    for(int i = 5;i>0;i--)
    {
        struct ListNode*p= (struct ListNode*)malloc(sizeof(struct ListNode));
        p->val=i;
        p->next=NULL;
        if(last == NULL)
        {
            head = p;
            last = p;
        }
        else
        {
            last->next=p;
        }
    }
    int n = 3;
    partition(head, n);


    return 0;
}