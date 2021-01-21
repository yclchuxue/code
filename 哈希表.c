#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ListNode{
    int value;
    char name[5];
    struct ListNode* next;
}ListNode;

void print(ListNode *head)
{
    ListNode *p = head;
    for(;p;p = p->next)
    {
        printf("%d\t%s\n",p->value,p->name);
    }
}

int haxi(int value)
{
    int n = value;
    int sum = 0;
    while(n>0)
    {
        sum = sum + n%10;
        n = n / 10;
    }

    return sum;
}

int main()
{
    int n;
    printf("请输入学生人数：\n");
    scanf("%d",&n);
    ListNode **H = (ListNode **)malloc(sizeof(*H)*100);
    memset(H, 0, sizeof(ListNode));
    ListNode *head = NULL;
    ListNode *tail = NULL;
    for(int i = 0;i<n;i++)
    {
        int val;
        char str[5];
        printf("请输入学生学号：\n");
        scanf("%d", &val);
        printf("请输入学生姓名：\n");
        scanf("%s", str);
        ListNode *p = (ListNode*)malloc(sizeof(ListNode));
        p->value = val;
        strcpy(p->name, str);
        p->next = NULL;
        int x = haxi(val);
        H[x] = p;
        if(!head)
        {
            head = p;
            tail = p;
        }
        else
        {
            tail->next = p;
        }
        tail = p;
    }
    print(head);
    int m;
    printf("请输入你要查询学生的学号:\n");
    scanf("%d", &m);
    int y = haxi(m);
    printf("%s",H[y]->name);

    return 0;
}