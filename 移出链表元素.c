 struct ListNode {
   int val;
    struct ListNode *next;
};

//1.0   超时
struct ListNode* removeElements(struct ListNode* head, int val)
{
    while(head->val == val)
    {
        head = head->next;
    }
    struct ListNode*last = head;
    last = last->next;
    struct ListNode*list = head;
    while(last)
    {
        while(last->val == val)
        {
            if(last->next->val == val)
            {
                last = last->next;
            }
        }
        list = last->next;
    }
    if(last->val == val)
    {
        list->next = last->next;
    }

    return head;
}


//2.0
struct ListNode* removeElements(struct ListNode* head, int val)
{
    struct ListNode *sentinel = malloc(sizeof(struct ListNode)); 
    sentinel->next = head; 
    struct ListNode *curr = head, *prev = sentinel; 
    while (curr)
    { 
        if (curr->val == val)
        { 
            prev->next = curr->next; 
        } 
        else
        { 
            prev = curr; 
        } 
        curr = curr->next; 
    } 
    return sentinel->next; 
}

//3.0  感觉不能处理两个val连在一起的情况
struct ListNode* removeElements(struct ListNode* head, int val)
{ 
    while(head&&head->val==val) 
    head=head->next;
    struct ListNode* tmp=head; 
    while(tmp)
    { 
        if(tmp->next&&tmp->next->val==val)
        { 
            tmp->next=tmp->next->next; 
        } 
        else
        { 
            tmp=tmp->next;
        } 
    } 
    return head; 
}
