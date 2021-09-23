/*****************************
 * 马踏棋盘---PTA
 * **************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct move{
    int m_x;
    int m_y;
}M[8] = {{-2,1},{-1,2},{1,2},{2,1},{2,-1},{1,-2},{-1,-2},{-2,-1}};

int Buf[9][9], num = 1;

int Isok(int x, int y, int n)
{
    int x1 = x+M[n].m_x;
    int y1 = y+M[n].m_y;
    if(x1 > 0 && x1 < 9 && y1 >0 && y1 < 9 && Buf[x1][y1] == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int xy_move(int x, int y, int n)
{
    int ice = 0, i, x1 = x, y1 = y;
    for(i = n; i < 8; ++i)
    {
        if(Isok(x,y,i))
        {
            x += M[i].m_x;
            y += M[i].m_y;
            ++num;
            Buf[x][y] = num;
            ice = 1;
            break;
        }
    }
    if(ice != 1)
    {
        return 0;
    }
    if(num == 64)
    {
        return 1;
    }
    if(xy_move(x,y,0) == 0)
    {
        Buf[x][y] = 0;
        --num;
        xy_move(x1,y1,i+1);
    }
}

int main()
{
    //printf("%d\t%d\n", M[7].m_x, M[7].m_y);
    int x, y;
    scanf("%d %d", &x, &y);
    Buf[x][y] = num;
    xy_move(x, y, 0);

    for(int i = 1; i <= 8; ++i)
    {
        for(int j = 1; j <= 8;++j)
        {
            printf("%2d ", Buf[i][j]);
        }
        printf("\n");
    }

    return 0;
}