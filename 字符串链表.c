#include<stdio.h>
#include<stdlib.h>
struct symbol{
    char sym;
    struct symbol *nextPtr;
};
struct symbol *createsym(struct symbol *p);
void listsym(struct symbol *p);
int main(){
    struct symbol *head;
    head=NULL;
    head=createsym(head);
    listsym(head);
    return 0;
}
struct symbol *createsym(struct symbol *p){
    //p=(struct symbol *)malloc(sizeof(struct symbol));
    struct symbol *p1,*p2;
    p1=p2=(struct symbol*)malloc(sizeof(struct symbol));
    printf("请输入字符串以enter结束：\n");
    scanf("%c",&p1->sym);
    p1->nextPtr=NULL;
    p=p1;
    while(p1->sym!='\n'){
        p2=p1;
        p1=(struct symbol *)malloc(sizeof(struct symbol));
        scanf("%c",&p1->sym);
        p1->nextPtr=NULL;
        p2->nextPtr=p1;
    }
    printf("输入完成！\n");
    return p;
}
void listsym(struct symbol *p){
    if(!p){
        printf("字母链表为空！\n");
    }else{
        printf("字母序列为：\n");
        while(p){
            printf("%c",p->sym);
            p=p->nextPtr;
        }
    }
}
