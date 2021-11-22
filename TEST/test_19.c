#include<stdio.h>
#include<stdlib.h>
#define MaxSize 100
typedef struct node{
    char data;
    struct node *Lchild;
    struct node *Rchild;
}BiNode, *BiTree;

//扩展先序创建二叉树
void Creat_Pre_Bitree(BiTree *root)
{
    char c;
    c = getchar();
    if(c=='#')
        *root = NULL;
    else
    {
        *root = (BiTree)malloc(sizeof(BiNode));
        (*root)->data = c;
        Creat_Pre_Bitree(&((*root)->Lchild));
        Creat_Pre_Bitree(&((*root)->Rchild));
    }
}

/*非递归*/
//栈相关
typedef struct {
    BiTree stdata[MaxSize];
    int top;
} sqstack;
//创建栈
sqstack* Init_Stack()
{
    sqstack *s;
    s = (sqstack *)malloc(sizeof(sqstack));
    s->top = -1;
    return s;
}

//判空
int IsEmpty(sqstack *s)
{
    if(s->top==-1)
        return 1;
    else
        return 0;
}

//入栈
void Push(sqstack*s,BiTree root){

    if(s->top<MaxSize){
        s->top++;
        s->stdata[s->top] = root;
        
    }
    return;
}

//出栈
int Pop(sqstack*s,BiTree *x){
    if(IsEmpty(s))
    {
        return 0;
    }
    else
    {
        *x = s->stdata[s->top];
        s->top--;
        return 1;
    }

}
//非递归先序
void LPreOrder(BiTree root)
{
    sqstack* s;
    s = Init_Stack();
    BiTree p;
    p = root;
    while(p!=NULL||!IsEmpty(s)){
        while(p!=NULL){
            printf("%c", p->data);
            Push(s, p);
            p = p->Lchild;
        }
        if(!IsEmpty(s)){
            Pop(s,&p);
            p = p->Rchild;
        }
    }

}
//非递归中序
void LInOrder(BiTree root)
{
    sqstack* s;
    s = Init_Stack();
    BiTree p = root;
    while(p!=NULL||!IsEmpty(s)){
        while(p!=NULL){
            Push(s, p);
            p = p->Lchild;
        }
        if(!IsEmpty(s)){
            Pop(s,&p);
            printf("%c", p->data);
            p = p->Rchild;
        }
    }

}


int main()
{
    BiTree root;
    Creat_Pre_Bitree(&root);
    LPreOrder(root);
    printf("\n");
    LInOrder(root);
    printf("\n");

    return 0;
}