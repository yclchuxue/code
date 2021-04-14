#include <stdio.h>
#include <stdlib.h>

struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

int k = 0,A[100],B[100];

int cmp(int* a,int* b)
{
    return *a - *b;
}

void digui1(struct TreeNode *T)
{
    if(T == NULL)
    {
        return;
    }
    B[k] = T->val;
    k++;
    digui1(T->left);
    digui1(T->right);
}

void digui2(int x,struct TreeNode *T)
{
    if(T == NULL)
    {
        return;
    }
    if(T->val - x < 0)
    {
        A[k] = x - T->val;
    }
    else
    {
        A[k] = T->val - x;
    }
    k++;
    digui2(x,T->left);
    digui2(x,T->right);
}

int minDiffInBST(struct TreeNode* root)
{
    struct TreeNode *T = root;
    int min,n;
    digui1(T);
    n = k;
    k = 0;
    for(int i = 0; i < n; i++)
    {
        digui2(B[i],T);
    }
    qsort(A,k,sizeof(int),cmp);
    printf("%d\t%d",A[n],A[n+1]);
    return 0;
}