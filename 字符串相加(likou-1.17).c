//1.0
char* addStrings(char* num1, char* num2) 
{
    int i = strlen(num1) - 1, j = strlen(num2) - 1, add = 0;
    char* ans = (char*)malloc(sizeof(char) * (fmax(i, j) + 3));
    int len = 0;
    while (i >= 0 || j >= 0 || add != 0) 
    {
        int x = i >= 0 ? num1[i] - '0' : 0;
        int y = j >= 0 ? num2[j] - '0' : 0;
        int result = x + y + add;
        ans[len++] = '0' + result % 10;
        add = result / 10;
        i--, j--;
    }
    for (int i = 0; 2 * i < len; i++) 
    {
        int t = ans[i];
        ans[i] = ans[len - i - 1], ans[len - i - 1] = t;
    }
    ans[len++] = 0;
    return ans;
}
//2.0
char * addStrings(char * num1, char * num2)
{
    int num1_len = strlen(num1)-1;
    int num2_len = strlen(num2)-1;
    int m = 0,n = 0;
    for(int i = num1_len-1; i >= 0;i--)
    {
        m = m*10 + num1[i];
    }
    for(int i = num2_len-1; i >= 0;i--)
    {
        n = n*10 + num1[i];
    }
    int s = 0,y = m+n,x = m+n;
    while(y>0)
    {
        s++;
        y = y/10;
    }
    char h[s];
    while(x>0)
    {
        h[s-1] = x%10;
        x = x/10;
        s--;
    }
    return h;
}
