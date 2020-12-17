struct ListNode {
    int val;
    struct ListNode *next;
};


//1.0  
struct ListNode* reverseList(struct ListNode* head)
{
    struct ListNode*p = head;
    p = p->next;
    head->next = NULL;
    while(p)
    {
        struct ListNode*t = p->next;
        p->next = head;
        head = p;
        p = t;
    }

    return head;
}


//2.0
struct ListNode* reverseList(struct ListNode* head)
{
    struct ListNode*p = head;
    struct ListNode*q = NULL;
    while(p->next)
    {
        struct ListNode*b = p->next;
        p->next = q;
        q = p;
        p = b;
    }

    return p;
}


//3.0  通过
struct ListNode* reverseList(struct ListNode* head)
{ 
    struct ListNode* pre = NULL; 
    while (head != NULL) 
    { 
        struct ListNode* next = head->next; 
        head->next = pre; 
        pre = head; 
        head = next; 
    } 
    return pre; 
}
