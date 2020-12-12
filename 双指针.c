/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */


struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) 
{
    struct ListNode*ret = NULL;
    int carry = 0;
    while(l1||l2||carry)
    {
        carry = l1->val + l2->val;
        struct ListNode*p = (struct ListNode*)malloc(sizeof(struct ListNode));
        if(ret == NULL)
        {
            ret = p;
        }
        p->val = carry%10;
        carry = carry/10;
        l1 = l1->next;
        l2 = l2->next;
    }

    return ret;
}


struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2){
            struct ListNode* tail = NULL;
            struct ListNode* head = NULL;
    for(; l1 && l2;l1 = l1->next, l2 = l2->next)
    {
        struct ListNode* l3 = (struct ListNode*)malloc(sizeof(struct ListNode));
        if(l1->val + l2->val < 10) l3->val = l1->val + l2->val;
        else 
        {
            l3->val = l1->val + l2->val - 10;
            if(l1->next == NULL && l2->next == NULL)
            {
                l3->next = NULL;
                if (head) tail->next = l3;
                else head = l3;
                tail = l3;
                struct ListNode* l3 = (struct ListNode*)malloc(sizeof(struct ListNode));
                l3->val = 1;
                l3->next = NULL;
                tail->next = l3;
                tail = l3;
                return head;
            }
            if (l1->next == NULL && l2->next != NULL)
            l2->next->val += 1;
            if (l2->next == NULL && l1->next != NULL)
            l1->next->val += 1;
            if (l2->next != NULL && l1->next != NULL)
            l1->next->val += 1;
        }
        l3->next = NULL;
        if(head) tail->next = l3;
        else head = l3;
        tail = l3;
    }
    if(l2 == NULL && l1 != NULL)
    {
        for(; l1; l1 = l1->next)
        {
            struct ListNode* l3 = (struct ListNode*)malloc(sizeof(struct ListNode)); 
            if(l1->val < 10) l3->val = l1->val;
            else
            {
                l3->val = l1->val - 10;
                if(l1->next == NULL)
                {
                    l3->next = NULL;
                    tail->next = l3;
                    tail = l3;
                    struct ListNode* l3 = (struct ListNode*)malloc(sizeof(struct ListNode));
                    l3->val = 1;
                    l3->next = NULL;
                    tail->next = l3;
                    tail = l3;
                    return head;
                }
                else
                l1->next->val += 1;
            }
            l3->next = NULL;
            tail->next = l3;
            tail = l3;
        }
    }
    if(l1 == NULL && l2 != NULL)
    {
        for(; l2; l2 = l2->next)
        {
            struct ListNode* l3 = (struct ListNode*)malloc(sizeof(struct ListNode)); 
            if(l2->val < 10) l3->val = l2->val;
            else
            {
                l3->val = l2->val - 10;
                if(l2->next == NULL)
                {
                    l3->next = NULL;
                    tail->next = l3;
                    tail = l3;
                    struct ListNode* l3 = (struct ListNode*)malloc(sizeof(struct ListNode));
                    l3->val = 1;
                    l3->next = NULL;
                    tail->next = l3;
                    tail = l3;
                    return head;
                }
                else
                l2->next->val += 1;
            }
            l3->next = NULL;
            tail->next = l3;
            tail = l3;
        }
    }
    return head;
}