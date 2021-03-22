#include <stdio.h>
#include <string.h>

//计算数组中含有的汉子的个数 
int calc_hz_count(const char * array)
{
    
    int sum = 0;
    int index = 0;
    while(array[index])
    {
        if (array[index] < 0)
        {
            sum++;
        }
        index++;
    }
    sum /= 3;
//    printf("sum = %d\n", sum / 3);

    return sum;
}

int main(int argc, const char *argv[])
{
    int counter = 0;
    const char *a = "me国士无双 钟南山aasjkfj123";

    counter = calc_hz_count(a);
    
    printf("hz counter = %d\n", counter);

    return 0;
}