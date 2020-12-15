
struct ListNode {
    int val;
    struct ListNode *next;
};


//1.0  因为链表每一个节点的数的位数不同所以这个方法不可以使用
int isPalindrome(struct ListNode* head)
{
    int sum = 0,n = 0;
    while(head)
    {
        sum = sum*10 + head->val;
        head = head->next;
    }
    int s = 0, i = sum;
    while(i)
    {
        s = s*10+i%10;
        i = i/10;
    }
    if(s == sum)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    
}


//2.0
int isPalindrome(struct ListNode* head)
{
    int A[1000] = {0}, i = 0,ice = 1;
    while(head)
    {
        A[i] = head->val;
        i++;
    }
    for(int k = 0;k<i;k++)
    {
        int j = i-1;
        if(A[k] != A[j])
        {
            ice = 0;
        j--;
    }
    return ice;
}