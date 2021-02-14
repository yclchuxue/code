#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

struct TreeNode* mergeTrees(struct TreeNode* t1, struct TreeNode* t2) 
{
    if (t1 == NULL) 
    {
        return t2;
    }
    if (t2 == NULL) 
    {
        return t1;
    }
    struct TreeNode* merged = malloc(sizeof(struct TreeNode));
    merged->val = t1->val + t2->val;
    merged->left = mergeTrees(t1->left, t2->left);
    merged->right = mergeTrees(t1->right, t2->right);
    return merged;
}


int main()
{
    int m,n,x = 1;
    printf("请输入两个二叉树的层数：\n");
    scanf("%d %d",&m,&n);
    while(x<=2)
    {

    }
}