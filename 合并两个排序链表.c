struct ListNode {
    int val;
    struct ListNode *next;
};

//1.0
struct ListNode* mergeTwoLists(struct ListNode* l1, struct ListNode* l2)
{
    struct ListNode*p1 = l1;
    struct ListNode*p2 = l2;
    struct ListNode*b = NULL;
    struct ListNode*b1;
    while(p1&&p2)
    {
        if(p1->val <= p2->val)
        {
            p1 = p1->next;
            if(!b)
            {
                b = l1;
                b1 = l1;
            }
            else
            {
                b1 = p1->next;
            }
            l1 = l1->next;
        }
        else
        {
            if(!b)
            {
                b = l2;
                b1 = l2;
            }
            else
            {
                b1 = p2->next;
            }
            
            l2 = l2->next;

        }
    }
    return b;
}


//2.0
struct ListNode *mergeTwoLists(struct ListNode *l1, struct ListNode *l2)
{ 
    if (l1 == NULL) 
    {
         return l2;
    } 
    if (l2 == NULL) 
    { 
        return l1; 
    }
    struct ListNode *p = NULL; 
    if (l1->val < l2->val) 
    { 
        p = l1; 
        p->next = mergeTwoLists(l1->next, l2); 
    }
    else 
    { 
        p = l2; 
        p->next = mergeTwoLists(l1, l2->next); 
    } 
    return p;
}