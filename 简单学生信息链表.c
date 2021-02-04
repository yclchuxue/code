#include <stdio.h>
#include <math.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#define format "%d\n%s\n%f\n%f\n%f\n"
#define len sizeof(stu)

typedef struct student{
    long num;
    float score;
    struct student *next;
}stu;

int n;
int Tips()
{
    int p;
    printf("<----------请选择需要的功能---------->\n");
    printf("<----------1.输入一个新的链表-------->\n");
    printf("<----------2.删除一个新的纪录-------->\n");
    printf("<----------3.插入一个新的记录-------->\n");
    printf("<----------4.显示全部学生成绩-------->\n");
    printf("<----------0.退出------------------->\n");
    scanf("%d",&p);
    return p;
 
}
     
     
    //创建学生信息
stu * creat()
{
    stu *p1,*p2,*head;int i,t,k=1;
    n=0;
    printf("输入学生信息的个数\n");
    scanf("%d",&t);
     
    head=0;
    for(i=0;i<t;i++)
    {
        p1=(stu *)malloc(len);
        printf("输入第%d个学生的信息\n",k++);
        printf("学号：");
        scanf("%ld",&p1->num);
        printf("成绩:");
        scanf("%f",&p1->score);
        n=n+1;
        if(n==1)
            head=p1;
        else
        p2->next=p1;
        p2=p1;
    }
    p2->next=0;
    return head;
}
     
     
     
    //删除学生信息
stu * del(stu * head,long num)
{
    stu *p1,*p2;
    if(head==0)
        printf("表空!\n");
    else
    {
        p1=head;
        while(p1->num!=num && p1->next!=0)
        {
            p2=p1;p1=p1->next;
        }
        if(p1->num==num)
        {
            if(p1==head)
            {
            head=p1->next;
            }
            else
            {
                p2->next=p1->next;
    
            }
            printf("删除学号:%ld成功\n",num);
            n=n-1;
        }
        else
            printf("没有%ld这个学号!\n",num);
    }
    return head;
}
     
     
     
     
     
     
    //插入学生信息
stu *insert(stu * head, stu * stud)
{
    stu *p1,*p2,*p0;
    p0=stud;p1=head;
    if(head==0)
    {
        head=p0;stud->next=0;
    }
    else
    {
        while(p1->num<p0->num && p1->next!=0)
        {
            p2=p1;p1=p1->next;
        }
        if(p1->num>=p0->num)
        {
            if(p1==head)
            {
                head=p0;
                p0->next=p1;
            }
            else
            {
                p2->next=p0;
                p0->next=p1;
            }
    
        }
        else
        {
            p1->next=p0,p0->next=0;
        }
     
    }
    n=n+1;
    return head;
}
     
     
     
    //打印学生信息
void print(stu * head)
{
    stu * p;
    printf("此时的%d个学生信息是:\n",n);
    p=head;
    if(head!=0)
    {
        while(p!=0)
       {
        printf("学号:%-10ld成绩:%f\n",p->num,p->score);
        p=p->next;
       }
    }
}
     
     
     
     
    //主程序执行
int main()
{
    stu * head=0,*stud;
    int m;
    long del_num;
    int i=1;
 
    while(i>0)
    {
        system("cls");
        i=Tips();
        switch(i)
        {
            case 1:
           {
                system("cls");
                printf("开始输入学生信息:\n");
                head=creat();
                system("cls");
                print(head);
                printf("请输入0返回\n");
                scanf("%d",&m);
                while(m!=0)
                {
                    printf("请输入0返回\n");
                    scanf("%d",&m);
                    }
                    break;
    
            }
            case 2:
            { 
                system("cls");
                printf("输入要删除的学号:");
                scanf("%ld",&del_num);
                head=del(head,del_num);
                print(head);
                printf("请输入0返回\n");
                scanf("%d",&m);
                while(m!=0)
                {
                    printf("请输入0返回\n");
                    scanf("%d",&m);
                }
                break;
            }
            case 3:
            {
                system("cls");
                stud=(stu *)malloc(len);
                printf("输入要增加的学生学号:");
                scanf("%ld",&stud->num);
                printf("输入要增加的学生成绩:");
                scanf("%f",&stud->score);
                head=insert(head,stud);
                print(head);
                printf("请输入0返回\n");
                scanf("%d",&m);
                while(m!=0)
                {
                    printf("请输入0返回\n");
                    scanf("%d",&m);
                }
                break;
            }
            case 4:
            {   
                system("cls");
                print(head);
                printf("请输入0返回\n");
                scanf("%d",&m);
                while(m!=0)
                {
                    printf("请输入0返回\n");
                    scanf("%d",&m);
                }
                break;
            }
        }
    }
}