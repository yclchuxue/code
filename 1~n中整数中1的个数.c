#include <stdio.h> 
#define MAX_HASH_LEN 11 
long g_hash[MAX_HASH_LEN] = {0,1}; 
void get_hash(long *hash) 
{ 
    int i; 
    int tmp = 1; 
    for (i = 2; i < MAX_HASH_LEN; i++) 
    { 
        tmp *= 10; 
        hash[i] = 10 * hash[i - 1] + tmp; 
    }
} 
int countDigitOne(int n)
{ 
    if (n <= 0 || n >= 2147483648) 
    return 0; 
    get_hash(g_hash); 
    long result = 0; 
    int times = 0; 
    long add = 1; 
    int add_for_one = 0; 
    int tmp = 0; 
    while (n) 
    { 
        tmp = n % 10; 
        if (tmp > 1) 
        result += tmp * g_hash[times] + add; 
        else if (tmp == 1) 
        result += tmp * g_hash[times] + add_for_one + 1; 
        else 
        result += tmp * g_hash[times]; 
        add_for_one += tmp * add; 
        times++; 
        add *= 10; 
        n /= 10; 
    } 
    return result; 
} 
