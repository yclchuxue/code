#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setZeroes(int** matrix, int matrixSize, int* matrixColSize)
{

}

int main()
{
    int A[100][100];
    int m, n;
    int **B = A;
    scanf("%d %d", &m, &n);
    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j < n; j++)
        {
            scanf("%d", &A[i][j]);
        }
    }
    setZeroes(B,m,&n);

    return 0;
}