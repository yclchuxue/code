#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node{
	int value;
	struct node *next;
}Node;

typedef struct list{
	Node*head;
	Node*last;
}List;

void creat(List*plist)
{
	plist->head = NULL;
	int m;
	do
	{
		scanf("%d",&m);
		if(m != -1)
		{
			Node *p = (Node*)malloc(sizeof(Node));
			p->value = m;
			p->next = NULL;
			if(plist->head == NULL)
			{
				plist->head = p;
			}
			else
			{
				plist->last->next = p;
			}
			plist->last = p;
		};
	}while(m != -1);
}

void print(List *plist)
{
	Node *p = plist->head;
	for(;p;p = p->next)
	{
		printf("%d\t",p->value);
	}
}


void shanchu(List *plist)
{
	printf("\n请输入你要删除的内容：");
	int n;
	scanf("%d", &n);
	Node*p = plist->head;
	Node*b = plist->head;
	for(p = p->next;p;p=p->next)
	{
		if(b->value == n)
		{
			free(b);
			Node*b = p;
			plist->head = p;
		}
		else if(p->value == n)
		{
			b->next = p->next;
			free(p);
			Node*p = b->next;
			b=b->next;
		}
	}
}

void fanzhuan(List *plist)
{
	printf("\n");
	Node *b = plist->head;
	Node *p = b->next;
	Node *q = p->next;
	b->next = NULL;
	for(;q;q = q->next)
	{
		p->next = b;
		b = p;
		p = q;
	}
	q->next = p;
	plist->head = q;
}
int main()
{
	List list;
	creat(&list);
	print(&list);
	shanchu(&list);
	print(&list);
	fanzhuan(&list);
	print(&list);

	return 0;
}
