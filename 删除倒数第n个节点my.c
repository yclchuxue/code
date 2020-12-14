/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

//1.
struct ListNode* removeNthFromEnd(struct ListNode* head, int n)
{
    struct ListNode*p;
    struct ListNode*q;
    struct ListNode*r;
    p = head;
    q = head;
    r = NULL;
    for(int i = 1;i != n;i++)
    {
        p = p->next;
    }
    while(p->next != NULL)
    {
        p = p->next;
        r = q;
        q = q->next;
    }
    if(r == NULL)
    {
        head = head->next;
    }
    else
        r = q->next;
    return head;
}


//2.
struct ListNode* removeNthFromEnd(struct ListNode* head, int n)
{ 
    struct ListNode *p, *q, *r; 
    int count = 0; 
    p = head; 
    q = head; 
    r = NULL; 
    while(count!=n-1)
    {
        p = p->next; 
        count++; 
    } 
    while(p->next!=NULL)
    {
        p = p->next; 
        r = q; 
        q = q->next; 
    } 
    if(r==NULL)
        head = head->next; 
    else 
        r->next = q->next; 
    return head; 
}