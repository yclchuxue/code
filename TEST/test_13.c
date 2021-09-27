/***********************
 * 走迷宫问题-----PTA
 * ********************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct move{
    int m_x;
    int m_y;
    int f;
    struct move *next;
}ret;

ret M[4] = {{0,1,1},{1,0,2},{0,-1,3},{-1,0,4}};
int S,B;
char A[50][50];

int Isok(int x, int y, int n, ret *q)
{
    //printf("Q");
    if(q != NULL)
    {
        if(q->f+2 == n+1 || q->f-2 == n+1)
        {
            //printf("x%d\t%d\n", x,y);
            return 0;
        }
    }
    int X = x + M[n].m_x - 1;
    int Y = y + M[n].m_y - 1;
    //printf("X = %d Y = %d\tA = %c\tS = %d\tB = %d\n", X, Y, A[X][Y],S,B);
    if(X >= 0 && X < S && Y >= 0 && Y < B && A[X][Y] == '0')
    {
        //printf("C");
        return 1;
    }
    else
    {
        //printf("y%d\t%d\n", x,y);
        return 0;
    }
}

int move_xy(int x, int y, int n, ret *p, ret *q)
{
    //sleep(4);
    int ice = 0, i;
    ret *h = (ret*)malloc(sizeof(ret));
    for(i = n; i < 4; ++i)
    {
        //printf("%d\t%d\t%d\t%d\n",x,y,i,Isok(x,y,i,q));
        if(Isok(x,y,i,q))
        {
            h->m_x = x + M[i].m_x;
            h->m_y = y + M[i].m_y;
            p->f   = M[i].f;
            A[h->m_x-1][h->m_y-1] = '1';
            p->next = h;
            h->next = NULL;
            ice = 1;
            break;
            //printf("A = %d\t%d\n", h->m_x, h->m_y);
            //printf("A");
        }
    }
    if(ice == 0)
    {
        return 0;
    }
    if(h->m_x == S && h->m_y == B)
    {
        return 1;
    }
    //printf("A = %d\t%d\n", h->m_x, h->m_y);
    
    if(!move_xy(h->m_x,h->m_y,0,h,p))
    {
        p->next = NULL;
        //A[h->m_x][h->m_y] = '0';
        //free(h);
        move_xy(p->m_x, p->m_y, i+1, p, q); 
    }
}

int main()
{
    int m,n;
    scanf("%d %d", &m, &n);
    S = m;
    B = n;
    for(int i = 0; i < m; ++i)
    {
        scanf("%s", &A[i]);
    }

    ret *head = (ret *)malloc(sizeof(ret));
    head->m_x = 1;
    head->m_y = 1;
    head->next = NULL;
    //printf("%c\n", A[2][2]);
    move_xy(1, 1, 0, head, NULL);

    while(head)
    {
        printf("(%d,%d,%d)", head->m_x, head->m_y, head->f);
        head = head->next;
    }
/*
    for(int i = 0; i < 4; ++i)
    {
        printf("%d\t%d\t%d\n", M[i].m_x, M[i].m_y, M[i].f);
    }
*/
    return 0;
}

