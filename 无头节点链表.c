#include <stdio.h>
#include <stdlib.h>

typedef struct Listnode{
    int val;
    struct Listnode *next;
}Listnode;


void Rprint(Listnode* L)   // 逆向输出链表函数
{
	if(L->next !=NULL)
	{
		Rprint(L->next);
	}
	printf("%d\t",L->val);
	return ;
}


void print(Listnode *head)   //正向输出链表函数
{
    Listnode *p = head;
    for(;p;p = p->next)
    {
        printf("%d\t",p->val);
    }
}


void weicha(Listnode *head)  //尾部插入
{
	Listnode *p = head;
	while(p->next != NULL)
	{
		p = p->next;
	}
	printf("请输入要插入尾部的数：\n");
	int n;
	scanf("%d",&n);
	Listnode *b = (Listnode *)malloc(sizeof(Listnode));
	b->val = n;
	b->next = NULL;
	p->next = b;
}

void shan(Listnode *head)                        // 删除链表的第k个节点
{
    printf("请输入要删除的节点数：\n");
    int k,i = 2;
    scanf("%d", &k);
    if(k == 1)
    {
        head = head->next;
    }
    else
    {
        Listnode *p = head->next;
        Listnode *q = head;
        while(i != k)
        {
            p = p->next;
            q = q->next;
            i++;
        }
        q->next = p->next;
    }
}

void weishan(Listnode *head)
{
    Listnode *p = head;
    Listnode *q = head;
    if(head->next != NULL)
    {
        p = p->next;
        for(;p->next; p = p->next)
        {
            q = q->next;
        }
        q->next = NULL;
    }
}

int main()
{
    int n;
    Listnode *head = NULL;
    Listnode *b = NULL;
    do                                                        // 创建无头节点链表
    {
        scanf("%d", &n);
        if(n != -1)
        {
            Listnode *p = (Listnode*)malloc(sizeof(Listnode));
            p->val = n;
            p->next = NULL;
            if(!head)
            {
                head = p;
                b = p;
            }
            else
            {
                b->next = p;
                b = b->next;
            }
        }
    }while(n != -1);
    print(head);
    printf("\n");
	weicha(head);
	print(head);
    printf("\n");
    weishan(head);
    print(head);
    printf("\n");
    shan(head);
    print(head);

    return 0;
}