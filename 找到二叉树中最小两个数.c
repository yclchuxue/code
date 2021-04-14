#include <stdio.h>
#include <stdlib.h>

struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

int min1 = 0,min2 = 0;

void digui1(struct TreeNode *T)
{
    if(T == NULL)
    {
        return;
    }
    if(T->val < min1)
    {
        min1 = T->val;
    }
    digui1(T->left);
    digui1(T->right);
}

void digui2(struct TreeNode *T)
{
    if(T == NULL)
    {
        return;
    }
    if(min1 == min2)
    {
        if(T->val > min2)
        {
            min2 = T->val;
        }
    }
    if(T->val < min2 && T->val > min1)
    {
        min2 = T->val;
    }
    digui2(T->left);
    digui2(T->right);
}

int minDiffInBST(struct TreeNode* root)
{
    struct TreeNode *T = root;
    int min;
    min1 = T->val;
    min2 = T->val;
    digui1(root);
    digui2(root);
    min = min2 - min1;
    printf("%d",min);

    return 0;
}