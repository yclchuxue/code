#include <stdio.h>
#include <stdlib.h>


int** largeGroupPositions(char* s, int* returnSize, int** returnColumnSizes) 
{
    *returnSize = 0;
    int n = strlen(s);
    int** ret = malloc(sizeof(int*) * (n / 3));
    *returnColumnSizes = malloc(sizeof(int) * (n / 3));
    int num = 1;
    for (int i = 0; i < n; i++) {
        if (i == n - 1 || s[i] != s[i + 1]) {
            if (num >= 3) {
                int* tmp = malloc(sizeof(int) * 2);
                tmp[0] = i - num + 1, tmp[1] = i;
                (*returnColumnSizes)[*returnSize] = 2;
                ret[(*returnSize)++] = tmp;
            }
            num = 1;
        } 
        else 
        {
            num++;
        }
    }
    return ret;
}

int main()
{
    char *s = "aabbbyyyycccchh";

}