#include<stdio.h>

 struct ListNode {
     int val;
     struct ListNode *next;
 };

int hasCycle(struct ListNode *head) 
{
    struct ListNode*p,*q;
    p = head;
    q = head;
    while(p != NULL)
    {
        p = p->next;
        if(p == NULL)
        {
            return 0;
        }
        p = p->next;
        if(p != q)
        {
            q = q->next;
        }
        else
        {
            return 1;
        }
    }
    return 0;
}
