typedef struct {
    char *email;
    int  idx;
    UT_hash_handle hh;
} Hash;

Hash *g_head = NULL;

/* 并查集find */
int find(int *parent, int x) {
    while (parent[x] != x) {
        x = parent[x];
    }
    return x;
}

/* 并查集合并 */
void merge(int *parent, int x, int y) {
    int root1 = find(parent, x);
    int root2 = find(parent, y);
    if (root1 == root2) {
        return;
    }
    parent[root1] = root2;
}

int strCmp(Hash *a, Hash *b) {
    return strcmp(a->email, b->email);
}

char *** accountsMerge(char *** accounts, int accountsSize, int* accountsColSize, int* returnSize, int** returnColumnSizes){
    int  i, j, idx, preIdx;
    int  *map     = (int*)malloc(sizeof(int) * accountsSize);
    int  *parent  = (int*)malloc(sizeof(int) * accountsSize);
    char ***res   = (char***)malloc(sizeof(char**) * accountsSize);
    int  *retCol  = (int*)malloc(sizeof(int) * accountsSize);
    int  retSize  = 0;
    char *email   = NULL;
    Hash *s   = NULL;
    Hash *tmp = NULL;
    g_head    = NULL;
    
    memset(retCol, 0, sizeof(int) * accountsSize);
    /* parent和map初始化 */
    for (i = 0; i < accountsSize; i++) {
        parent[i] = i;
        map[i]    = -1;
        res[i]    = (char**)malloc(sizeof(char*) * 10000);
    }
    
    /* 将邮件存入hash中, idx为val值 */
    for (i = 0; i < accountsSize; i++) {
        for (j = 1; j < accountsColSize[i]; j++) {
            email = accounts[i][j];
            HASH_FIND_STR(g_head, email, s);
            if (s == NULL) {
                s = malloc(sizeof(Hash));
                s->email = email;
                s->idx   = i;
                HASH_ADD_STR(g_head, email, s);
            } else {
                merge(parent, i, s->idx);
            }
        }
    }
    /* 字符串排序 */
    HASH_SORT(g_head, strCmp);
    
    /* 遍历hash表, 将对应email存入对应的res数组中 */
    HASH_ITER(hh, g_head, s, tmp) {
        idx = find(parent, s->idx);
        if (map[idx] == -1) {   /* 并查集未出现过, 新的下标存储 */
            res[retSize][retCol[retSize]++] = accounts[idx][0];
            res[retSize][retCol[retSize]++] = s->email;
            map[idx] = retSize++;
        } else {               /* 属于某个并查集, 存储在老的下标下 */
            preIdx = map[idx];
            res[preIdx][retCol[preIdx]++] = s->email;
        }
    }
    *returnSize        = retSize;
    *returnColumnSizes = retCol;
    return res;
}
