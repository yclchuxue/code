#include <stdio.h>

void xuanzhe(int A[],int n)
{
    for(int i = 0;i<n;i++)
    {
        int min = i;
        for(int j = i + 1;j<n;j++)
        {
            if(A[min]>A[j])
            {
                min = j;
            }
        }
        int t = A[min];
        A[min] = A[i];
        A[i] = t;
    }
}

int main()
{
    int A[10] = {8,5,2,3,7,6,4,1,9,0};
    xuanzhe(A,10);
    for(int i = 0;i<10;i++)
    {
        printf("%d\t",A[i]);
    }
<<<<<<< HEAD
}
=======
}
>>>>>>> 6f7e9c42e52d86d1ec0c4f4e6d23c7fa64a40feb
