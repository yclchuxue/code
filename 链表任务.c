#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Listnode{
    int value;
    struct Listnode* next;
}Listnode;

int tip();

Listnode *create(int n);        //创建链表

void print(Listnode *head);         //显示整个链表

Listnode *fanzhuan(Listnode *head);      //反转链表

Listnode *shan_i(Listnode *head,int i);        //删除第i个节点

Listnode *shan_key(Listnode *head,int key);      //删除value为key的所有节点

void cha_i(Listnode *head,int i);        //查找第i个节点

void cha_key(Listnode *head,int key);      //查找value为key的所有节点

Listnode *add_q(Listnode *head,int key);        //头插

void add_h(Listnode *head,int key);        //尾插

void xiugai(Listnode *head,int i);       //修改第i节点内容

int main()
{
    Listnode *head = NULL;
    int i = 1;
    int m;
    while(i>0)
    {
        system("cls");
        i = tip();
        switch(i)
        {
            case 1:
            {
                system("cls");
                printf("请输入你要创建链表节点的个数：\n");
                int n;
                scanf("%d",&n);
                head = create(n);
                printf("创建完成请输入0返回：\n");
                scanf("%d",&m);
                while(m != 0)
                {
                    printf("请输入0返回\n");
                    scanf("%d",&m);
                }
                break;
            }
            case 2:
            {
                system("cls");
                print(head);
                printf("打印完成请输入0返回：\n");
                scanf("%d",&m);
                while(m != 0)
                {
                    printf("\n请输入0返回\n");
                    scanf("%d",&m);
                }
                break;
            }
            case 3:
            {
                system("cls");
                head = fanzhuan(head);
                printf("反转后的链表为:\n");
                print(head);
                printf("反转完成请输入0返回：\n");
                scanf("%d",&m);
                while(m != 0)
                {
                    printf("\n请输入0返回\n");
                    scanf("%d",&m);
                }
                break;
            }
            case 4:
            {
                system("cls");
                int x;
                printf("请输入要查询的节点数：\n");
                scanf("%d",&x);
                printf("链表中第i个节点内容为：");
                cha_i(head,x);
                printf("查找完成请输入0返回：\n");
                scanf("%d",&m);
                while(m != 0)
                {
                    printf("请输入0返回\n");
                    scanf("%d",&m);
                }
                break;
            }
            case 5:
            {
                system("cls");
                int key;
                printf("请输入key：\n");
                scanf("%d",&key);
                cha_key(head,key);
                printf("查找完成请输入0返回：\n");
                scanf("%d",&m);
                while(m != 0)
                {
                    printf("请输入0返回\n");
                    scanf("%d",&m);
                }
                break;
            }
            case 6:
            {
                system("cls");
                int x;
                printf("请输入要删除节点序号：\n");
                scanf("%d",&x);
                Listnode *p = shan_i(head,x);
                printf("删除后的链表为：");
                print(p);
                printf("删除完成请输入0返回：\n");
                scanf("%d",&m);
                while(m != 0)
                {
                    printf("请输入0返回\n");
                    scanf("%d",&m);
                }
                break;                
            }
            case 7: 
            {
                system("cls");
                int key;
                printf("请输入要删除的key：\n");
                scanf("%d",&key);
                head = shan_key(head,key);
                printf("删除后的链表为：");
                print(head);
                printf("删除完成请输入0返回：\n");
                scanf("%d",&m);
                while(m != 0)
                {
                    printf("请输入0返回\n");
                    scanf("%d",&m);
                }
                break;
            }
            case 8: 
            {
                system("cls");
                int key;
                printf("请输入要插入的值key：\n");
                scanf("%d",&key);
                head = add_q(head,key);
                printf("插入后的链表为：");
                print(head);
                printf("插入完成请输入0返回：\n");
                scanf("%d",&m);
                while(m != 0)
                {
                    printf("请输入0返回\n");
                    scanf("%d",&m);
                }
                break;  
            }
            case 9: 
            {
                system("cls");
                int key;
                printf("请输入要插入的值key：\n");
                scanf("%d",&key);
                add_h(head,key);
                printf("插入后的链表为：");
                print(head);
                printf("插入完成请输入0返回：\n");
                scanf("%d",&m);
                while(m != 0)
                {
                    printf("请输入0返回\n");
                    scanf("%d",&m);
                }
                break;
            }
            case 10:
            {
                system("cls");
                int x;
                printf("请输入要修改的节点序号：\n");
                scanf("%d",&x);
                xiugai(head,x);
                printf("修改后的链表为：");
                print(head);
                printf("修改完成请输入0返回：\n");
                scanf("%d",&m);
                while(m != 0)
                {
                    printf("请输入0返回\n");
                    scanf("%d",&m);
                }
                break;
            }
        }

    }
}

int tip()
{
    int p;
    printf("<--------请选择需要的功能-------------------->\n");
    printf("<--------1.创建一个新链表------------------>\n");
    printf("<--------2.显示整个链表内容用->隔开---------->\n");
    printf("<--------3.反转整个链表并显示整个链表--------->\n");
    printf("<--------4.查找链表中第i个节点的内容--------->\n");
    printf("<--------5.查找所以值为key的节点序号--------->\n");
    printf("<--------6.删除链表中第i个节点-------------->\n");
    printf("<--------7.删除所有值为key的节点------------>\n");
    printf("<--------8.在链表头部插入一个节点------------>\n");
    printf("<--------9.在链表尾部插入一个节点------------>\n");
    printf("<--------10.修改第i个节点的内容-------------->\n");
    printf("<--------0.返回菜单或退出------------------->\n");
    scanf("%d", &p);
    return p;
}

Listnode *create(int n)        //创建链表
{
    Listnode *head = NULL;
    Listnode *tail = NULL;
    int i = 0,val;
    printf("请输入链表：\n");
    while(i<n)
    {
        scanf("%d",&val);
        Listnode *p = (Listnode*)malloc(sizeof(Listnode));
        p->value = val;
        p->next = NULL;
        if(head == NULL)
        {
            head = p;
            tail = p;
        }
        else
        {
            tail->next = p;
            tail = p;
        }
        i++;
    }
    return head;
}

void print(Listnode *head)         //现实整个链表
{
    Listnode *p = head;
    for(;p;p=p->next)
    {
        printf("%d",p->value);
        if(p->next)
        {
            printf("->");
        }
    }
}

Listnode *fanzhuan(Listnode *head)      //反转链表
{
    Listnode* pre = NULL; 
    while (head != NULL) 
    { 
        struct Listnode* next = head->next; 
        head->next = pre; 
        pre = head; 
        head = next; 
    } 
    return pre;
}

void cha_i(Listnode *head,int i)        //查找第i个节点
{
    Listnode *p = head;
    if(i == 1)
    {
        printf("%d\n",p->value);
    }
    else
    {
        int x = 1;
        while(x != i)
        {
            p = p->next;
            x++;
        }
        printf("%d\n",p->value);
    } 
}

void cha_key(Listnode *head,int key)      //查找value为key的所有节点
{
    printf("值为key的节点下标为：");
    Listnode *p = head;
    for(int x = 1;p;p = p->next)
    {
        if(p->value == key)
        {
            printf("\t%d",x);
        }
        x++;
    }
}

Listnode *shan_i(Listnode *head,int i)        //删除第i个节点
{
    if(i == 1)
    {
        head = head->next;
    }
    else
    {
        int x = 2;
        Listnode *p = head->next;
        Listnode *q = head;
        while(x != i)
        {
            p = p->next;
            q = q->next;
            x++;
        }
        q->next = p->next;
    }

    return head;
}

Listnode *shan_key(Listnode *head,int key)      //删除value为key的所有节点
{
    Listnode *p = head->next;
    Listnode *q = head;
    for(int x = 1;p;p = p->next)
    {
    if(head->value == key)
    {
        head = head->next;
    }
    if(p->value == key)
    {
        q->next = p->next;
    }
    q = q->next;
    }
    return head;
}

Listnode *add_q(Listnode *head,int key)        //头插
{
    Listnode *p = (Listnode*)malloc(sizeof(Listnode));
    p->value = key;
    p->next = head;
    head = p;

    return head;
}

void add_h(Listnode *head,int key)        //尾插
{
    Listnode *p = head;
    while(p->next)
    {
        p = p->next;
    }
    Listnode *q = (Listnode*)malloc(sizeof(Listnode));
    q->value = key;
    q->next = NULL;
    p->next = q;
}

void xiugai(Listnode *head,int i)       //修改节点内容
{
    Listnode *p = head;
    int key;
    for(int x = 1;p;p = p->next)
    {
        if(x == i)
        {
            printf("请输入新的内容：");
            scanf("%d",&key);
            p->value = key;
        }
        x++;
    }
}