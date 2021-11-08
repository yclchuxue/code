/************************************
 * 二叉树的创建及递归遍历
 * **********************************/
#include <stdio.h>
#include <stdlib.h>

typedef struct node{
	char ch;
	struct node *lchild;
	struct node *rchild;
}Node;

void get_tree( Node **p)
{
	char ch = getchar();
	
	if(ch == '#')
	{
		*p = NULL;
	}
	else
	{
		*p = (Node *)malloc(sizeof(Node));
		
		(*p)->ch = ch;
		get_tree(&((*p)->lchild));
	
		get_tree(&((*p)->rchild));
	}
	
}

void first_print(Node *head)
{
	if(head != NULL)
	{
		printf("%c", head->ch);
		first_print(head->lchild);
		first_print(head->rchild);
	}
}

void in_print(Node *head)
{
	if(head != NULL)
	{
		in_print(head->lchild);
		printf("%c", head->ch);
		in_print(head->rchild);
	}
}

void post_print(Node *head)
{
	if(head)
	{
		post_print(head->lchild);	
		post_print(head->rchild);
		printf("%c", head->ch);
	}
}

int main()
{
	char a[100];
	int i =0, n = 0;
	/*
	while(1)
	{
		scanf("%c", &a[i]);
		if(a[i] == '\n')
			break;
		++i;
	}
	n = i;
	*/
	Node *head = NULL;
	get_tree(&head);

	//printf("1 = %c", head->ch);
	//printf("2 = %c", head->lchild->ch);
	first_print(head);
	printf("\n");
	in_print(head);
	printf("\n");
	post_print(head);
	
	return 0;
}