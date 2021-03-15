#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*
int* spiralOrder(int** matrix, int matrixSize, int* matrixColSize, int* returnSize)
{
    *returnSize = matrixSize * matrixColSize[0];
    int i = 0, j = 0, k = 0;
    int *A = malloc(sizeof(int)*(*returnSize));
    while(matrixSize >= 0 && *matrixColSize >= 0 && k < *returnSize)
    {
        for( ; j < *matrixColSize; j++)
        {
            A[k] = matrix[i][j];
            k++;
        }
        matrixSize--;
        for(i++ ; i < matrixSize; i++)
        {
            A[k] = matrix[i][j];
            k++;
        }
        *matrixColSize--;
        for(j-- ;j >= 0; j--)
        {
            A[k] = matrix[i][j];
            k++;
        }
        matrixSize--;
        for(i-- ; i >= 0; i--)
        {
            A[k] = matrix[i][j];
            k++;
        }
        *matrixColSize--;
    }

    return A;
}

int* spiralOrder(int** matrix, int matrixSize, int* matrixColSize, int* returnSize) {
    if (matrixSize == 0 || matrixColSize[0] == 0) 
    {
        *returnSize = 0;
        return NULL;
    }

    int rows = matrixSize, columns = matrixColSize[0];
    int total = rows * columns;
    int* order = malloc(sizeof(int) * total);
    *returnSize = 0;

    int left = 0, right = columns - 1, top = 0, bottom = rows - 1;
    while (left <= right && top <= bottom) 
    {
        for (int column = left; column <= right; column++) 
        {
            order[(*returnSize)++] = matrix[top][column];
        }
        for (int row = top + 1; row <= bottom; row++) 
        {
            order[(*returnSize)++] = matrix[row][right];
        }
        if (left < right && top < bottom) 
        {
            for (int column = right - 1; column > left; column--) 
            {
                order[(*returnSize)++] = matrix[bottom][column];
            }
            for (int row = bottom; row > top; row--) 
            {
                order[(*returnSize)++] = matrix[row][left];
            }
        }
        left++;
        right--;
        top++;
        bottom--;
    }
    return order;
}
*/
int main()
{
    int i, j, m, n,k;
    /*scanf("%d %d",&m, &n);
    int **matrix;
    for(i= 0 ;i < m ;i++)
    {
        for(j= 0 ;j < n ;j++)
        {
            scanf("%d",&matrix[i][j]);
        }
    }*/
    k = 4;
    m = 2;
    n = 2;
    int **matrix = malloc(sizeof(int) *4);
    matrix[0][0] = 1;
    matrix[0][1] = 2;
    matrix[1][0] = 3;
    matrix[1][1] = 4;
    printf("A");
    int A[4] = {1, 2, 4, 3};
    //int *A = spiralOrder(matrix, m, &n ,NULL);
    for(i = 0 ; i < k ; i++)
    {
        printf("%d\t",A[i]);
    }

    return 0;
}