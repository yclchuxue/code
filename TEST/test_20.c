/*******************************
 * 二叉树的建立及非递归遍历
 * *****************************/
#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    char ch;
    struct node *lchild;
    struct node *rchild;;
}Node;

typedef struct {
    Node* data[100];
    int top;
}stack;

Node *get_tree()
{
    Node *p;
    char ch;
    scanf("%c", &ch);

    if(ch == '#')
    {
        p = NULL;
    }
    else
    {
        p = (Node *)malloc(sizeof(Node));
        p->ch = ch;
        p->lchild = get_tree();
        p->rchild = get_tree();
    }

    return p;
}

//栈初始化
stack* stack_init()
{
    stack *p = (stack*)malloc(sizeof(stack));

    p->top = -1;

    return p;
}

//判栈空
int is_stack(stack * p)
{   
    if(p->top == -1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int Push(stack *p, Node *q)
{
    if(p->top < 100)
    {
        p->top++;
        p->data[p->top] = q;
        return 1;
    }
    return 0;
}

int Pop(stack *p, Node **q)
{
    if(!is_stack(p))
    {
        *q = p->data[p->top];
        p->top--;
        return 1;
    }
    return 0;
}

void first_print(Node *q)
{
    stack *p = stack_init();

    while(q != NULL || !is_stack(p))
    {
        while(q != NULL)
        {
            printf("%c", q->ch);
            Push(p, q);
            q = q->lchild;
        }
        if(!is_stack(p))
        {
            Pop(p, &q);
            q = q->rchild;
        }
    }
}

void in_print(Node *q)
{
    stack *p = stack_init();

    while(q != NULL || !is_stack(p))
    {
        while(q != NULL)
        {
            Push(p, q);
            q = q->lchild;
        }
        if(!is_stack(p))
        {
            Pop(p, &q);
            printf("%c", q->ch);
            q = q->rchild;
        }
    }
}

int main()
{
    Node *head = get_tree();
    //printf("C");
    first_print(head);
    printf("\n");
    in_print(head);
    return 0;
}

