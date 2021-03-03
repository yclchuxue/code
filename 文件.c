#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#define N 2

struct stu{
    char name[10];
    int num;
    int age;
    float score;
} boya[N], boyb[N], *pa, *pb;


int main()
{
    FILE *fp;
    int i;
    pa=boya;
    pb=boyb;
    if( (fp=fopen("D:\\demo.txt","wt+")) == NULL )
    {
        puts("Fail to open file!");
        exit(0);
    }
    //从键盘读入数据，保存到boya
    printf("Input data:\n");
    for(i=0; i<N; i++,pa++)
    {
        scanf("%s %d %d %f", pa->name, &pa->num, &pa->age, &pa->score);   
    }
    pa = boya;
    //将boya中的数据写入到文件
    for(i=0; i<N; i++,pa++)
    {
        fprintf(fp,"%s %d %d %f\n", pa->name, pa->num, pa->age, pa->score);   
    }
    //重置文件指针
    rewind(fp);
    //从文件中读取数据，保存到boyb
    for(i=0; i<N; i++,pb++)
    {
        fscanf(fp, "%s %d %d %f\n", pb->name, &pb->num, &pb->age, &pb->score);
    }
    pb=boyb;
    //将boyb中的数据输出到显示器
    for(i=0; i<N; i++,pb++)
    {
        printf("%s  %d  %d  %f\n", pb->name, pb->num, pb->age, pb->score);
    }
    fclose(fp);
    return 0;
}

/*单个字符输入输出 */
/*简单打开文件并用fputc和fgetcxie读文件 并用feof检查是否结束*/
int main1 ()
{
    FILE * fd = NULL;
    char ch = ' ';
    char sh = ' ';
    fd = fopen("xiaofei.txt", "w");        //以w打开文件
    for(int i = 0;i<10;i++)
    {
        ch = getchar();                    //将字符交给ch
        fputc(ch, fd);                     //将ch写入文件
    }
    fclose(fd);                            //关闭文件
    fd = fopen("xiaofei.txt", "r");
    int i = 0;
    while(i != 1)
    {
        sh = fgetc(fd);                    //将fd指的一个字符给sh
        printf("%c", sh);                  //输出sh
        i = feof(fd);                      //检查指针fd是否指向文本结束
    }
    fclose(fd);                            //关闭文件

    return 0;
}

/*字符串输入输出 */
/*用fputs和fgets输入输出文件中的内容 */
/*注意fputs和fgets还有其他用法 */
int main2()
{
    FILE * fp;
    char ch[100];
    char sh[100];
    fp = fopen("xiaofei.txt", "w");
    fgets(ch,100,stdin);
    fputs(ch,fp);
    fclose(fp);
    fp = fopen("xiaofei.txt", "r");
    fgets(sh, 100, fp);
    fputs(sh,stdout);
    fclose(fp);

    return 0;
}


/*使用fwrite和fread两个函数进行文件的数据块输入/输出 */
int main3()
{
    FILE *fp;
    char ch[100];
    char sh[100];
    fp = fopen("xiaofei.txt","w");
    fgets(ch,100,stdin);                           //字符串输入
    fwrite(ch,50,2,fp);                            //将字符串ch存入文件
    fclose(fp);
    fp = fopen("xiaofei.txt", "r");
    fread(sh,100,1,fp);                            //将文件中的二进制读出存在sh中
    fputs(sh,stdout);                              //输出sh
    fclose(fp);

    return 0;
}