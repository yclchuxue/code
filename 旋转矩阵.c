#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void rotate(int** matrix, int matrixSize, int* matrixColSize)
{
    for(int i = 0; i < *matrixColSize; i++)
    {
        for(int j = matrixSize-1; j >= 0; j--)
        {
            printf("%d ",matrix[j][i]);
        }
        printf("\n");
    }
}

int main()
{
    int A[100][100];
    int m,n;
    scanf("%d %d",&m,&n);
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {
            scanf("%d",&A[i][j]);
        }
    }
    rotate(A,m,&n);

    return 0;
}