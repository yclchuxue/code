#include<stdio.h>

struct ListNode {
    int val;
    struct ListNode *next;
 };

//超时
struct ListNode *getIntersectionNode(struct ListNode *headA, struct ListNode *headB)
{
    struct ListNode*p = headA;
    struct ListNode*q = headB;
    struct ListNode*b;
    while(p != NULL)
    {
        for(;q;q=q->next)
        {
            if(p == q)
            {
                b = q;
            }
        }
    }
    return b;
}


//2.0
struct ListNode *getIntersectionNode(struct ListNode *headA, struct ListNode *headB)
{
    struct ListNode*p = headA;
    struct ListNode*q = headB;
    while(p != q)
    {
        if(p != NULL)
        {
            p=p->next;
        }
        else
        {
            p = headB;
        }
        if(q != NULL)
        {
            q = q->next;
        }
        else
        {
            q = headA;
        }
    }
    return q;
}