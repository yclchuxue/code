struct ListNode {
    int val;
    struct ListNode *next;
};


struct ListNode *getIntersectionNode(struct ListNode *headA, struct ListNode *headB) 
{
    struct ListNode *p = headA;
    struct ListNode *q = headB;
    while(p = q)
    {
        p = p->next;
        q = q->next;
        if(!p)
        {
            p = headB;
        }
        if(!q)
        {
            q = headA;
        }
    }

    return p;
}