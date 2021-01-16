#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Listnode{
    int val;
    char *name;
    struct Listnode *next;
}Listnode;

void print(Listnode *head)
{
    Listnode *p = head;
    for(;p;p = p->next)
    {
        printf("%d\t",p->val);
        fputs(p->name,stdout);
    }
}

int haxi(int val)
{
    int sum = 0;
    while(val>0)
    {
        sum = sum + val%10;
        val = val/10;
    }
    return sum;
}


int main()
{
    Listnode **list;
    int n = 1,m;
    Listnode *head = NULL;
    Listnode *b = NULL;
    scanf("%d",&m);
    while(n<=m)
    {
        int val;
        char na[5];
        printf("请输入学生证号码和学生姓名：\n");
        scanf("%d",&val);
        fgets(na,5,stdin);
        //int ha = haxi(val);
        Listnode *p = (Listnode*)malloc(sizeof(Listnode));
        p->val = n;
        strcpy(p->name,na);
        //list[ha] = p;
        p->next = NULL;
        printf("a");
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
        n++;
    }
    print(head);
    printf("\n");

    return 0;
}
