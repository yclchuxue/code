#include <stdio.h>
#include <stdlib.h>

struct ListNode{
    int val;
    struct ListNode*next;
};

void creat(struct ListNode*head);
void print(struct ListNode*head);

int main()
{
    struct ListNode*head;
    creat(head);
    print(head);

    return 0;
}

void creat(struct ListNode*head)
{
    struct ListNode*tail = NULL;
    int n;
    do
    {
        scanf("%d",&n);
        if(n != -1)
        {
            struct ListNode *p = (struct ListNode*)malloc(sizeof(struct ListNode));
            p->val = n;
            p->next = NULL;
            if(tail == NULL)
            {
                tail = p;
                head = p;
            }
            else
            {
                tail->next = p;
            }
        }
    }while(n != -1);
}

void print(struct ListNode*head)
{
    struct ListNode*p = head;
    printf("链表内容为：\n");
    while(p)
    {
        printf("%d\t",p->val);
        p = p->next;
    }
    printf("\n");
}
