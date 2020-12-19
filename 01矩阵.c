 typedef struct { 
    int x; 
    int y; 
    int level; 
}Queue; 
#define MAX_NUM 10000 
int BFS(int** matrix, int matrixSize, int* matrixColSize, Queue *que, int head, int tail)
{ 
    int size; 
    int i; 
    int n = *matrixColSize; 
    int m = matrixSize;
    Queue q; 
    int level = 0; 
    bool hasZero = false; 
    printf("    BFS:head %u tail %u\n", head, tail); 
    while (head != tail) 
    { 
        size = tail - head; 
        for (i = 0; i < size; i++) 
        { 
            memcpy(&q, &que[head], sizeof(Queue)); 
            printf("    que[%u]:%u (%u,%u)\n", head, matrix[q.y][q.x],q.y,q.x); 
            head++; 
            if (q.x + 1 < n && q.x + 1 >= 0) 
            { 
                if (matrix[q.y][q.x + 1] == 1) 
                { 
                    que[tail].x = q.x + 1; 
                    que[tail].y = q.y; 
                    tail++; 
                    } 
                else 
                { 
                    hasZero = true; 
                } 
            } 
            if (q.x - 1 >= 0 && q.x - 1 < n ) 
            { 
                if (matrix[q.y][q.x - 1] == 1) 
                { 
                   que[tail].x = q.x - 1; 
                  que[tail].y = q.y; 
                tail++; 
            } 
            else 
            { 
                hasZero = true;
            } 
            } 
            if (q.y + 1 < m && q.y + 1 >= 0) 
            { 
                if (matrix[q.y + 1][q.x] == 1) 
                { 
                    que[tail].x = q.x; 
                    que[tail].y = q.y + 1; 
                    tail++; 
                } 
                else 
                { 
                    hasZero = true; 
                } 
            } 
            if (q.y - 1 >= 0 && q.y - 1 < m) 
            { 
                if (matrix[q.y - 1][q.x] == 1) 
                { 
                    que[tail].x = q.x; 
                    que[tail].y = q.y - 1; 
                    tail++; 
                } 
                else 
               { 
                   hasZero = true; 
                } 
            } 
        } 
        level++;
        if (hasZero == true) 
        { 
        break; 
        } 
    } 
    return level; 
    } 
int** updateMatrix(int** matrix, int matrixSize, int* matrixColSize, int* returnSize, int** returnColumnSizes)
{ 
    int **returnMatrix = (int **)malloc(sizeof(int *) * matrixSize); 
    Queue que[MAX_NUM] = { 0 }; 
    int i,j; 
    int head = 0, tail = 0; 
    int m = matrixSize; 
    int n = *matrixColSize; 
    int *columnSizes = (int *)malloc(sizeof(int) * m); 
    for (i = 0; i < m; i++) 
    { 
        returnMatrix[i] = (int *)malloc(sizeof(int) * n);
        columnSizes[i] = n; } *returnSize = m; 
        *returnColumnSizes = columnSizes; 
        printf("matrixSize %u matrixColSize %u\n", matrixSize, *matrixColSize); 
        for (i = 0; i < m; i++) 
        { 
            printf("i:%u", i); 
            for (j = 0; j < n; j++) 
            { 
                printf("matrix[%u][%u]:%u\n", j, i,matrix[i][j]); 
                head = 0; 
                tail = 0; 
                if (matrix[i][j] == 0) 
                { 
                    returnMatrix[i][j] = 0;
                     //
                    printf("matrix[i][j] == 0\n"); 
                } 
                else 
                { 
                    que[tail].x = j; 
                    que[tail].y = i; 
                    que[tail].level = 0; 
                    tail++; 
                    returnMatrix[i][j] = BFS(matrix, matrixSize, matrixColSize, que, head, tail); 
                }
            } 
        } 
    return returnMatrix; 
} 
