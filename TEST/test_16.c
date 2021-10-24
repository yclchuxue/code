#include <stdio.h>

#define SWAP(a, b, t)  {         \
                        t = a;  \
                        a = b;  \
                        b = t;  \
                        }       \

/*
#define Swap(a, b)  {                \
                        int t = a;   \
                        a = b;       \
                        b = t;       \
                        }            \
*/

void swap(int a, int b)
{
    int t = a;
    a = b;
    b = t;
}

int main(int argc, char *argv[])
{
    int a = 10;
    int b = 20;
    
    int t;

    SWAP(a, b, t);

    printf("%d\n%d\n", a, b);

    swap(a, b);

    printf("%d\n%d\n", a, b);

    double val_1 = 12.123, val_2 = 98.12345, n;
    printf("val_1 = %lf\nval_2 = %lf\n", val_1, val_2);
    SWAP(val_1, val_2, n);
    printf("val_1 = %lf\nval_2 = %lf\n", val_1, val_2);

    return 0;
}

