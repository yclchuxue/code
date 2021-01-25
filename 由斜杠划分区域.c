void swap(int* a, int* b) 
{
    int tmp = *a;
    *a = *b, *b = tmp;
}

struct DisjointSetUnion 
{
    int *f, *size;
    int n, setCount;
};

void initDSU(struct DisjointSetUnion* obj, int n) 
{
    obj->f = malloc(sizeof(int) * n);
    obj->size = malloc(sizeof(int) * n);
    obj->n = n;
    obj->setCount = n;
    for (int i = 0; i < n; i++) 
    {
        obj->f[i] = i;
        obj->size[i] = 1;
    }
}

int find(struct DisjointSetUnion* obj, int x) 
{
    return obj->f[x] == x ? x : (obj->f[x] = find(obj, obj->f[x]));
}

int unionSet(struct DisjointSetUnion* obj, int x, int y) 
{
    int fx = find(obj, x), fy = find(obj, y);
    if (fx == fy) 
    {
        return 0;
    }
    if (obj->size[fx] < obj->size[fy]) 
    {
        swap(&fx, &fy);
    }
    obj->size[fx] += obj->size[fy];
    obj->f[fy] = fx;
    obj->setCount--;
    return 1;
}

struct HashTable 
{
    int val;
    UT_hash_handle hh;
};

int regionsBySlashes(char** grid, int gridSize) 
{
    int n = gridSize;
    struct DisjointSetUnion* uf = malloc(sizeof(struct DisjointSetUnion));
    initDSU(uf, n * n * 4);

    for (int i = 0; i < n; i++) 
    {
        for (int j = 0; j < n; j++) 
        {
            int idx = i * n + j;
            if (i < n - 1) 
            {
                int bottom = idx + n;
                unionSet(uf, idx * 4 + 2, bottom * 4);
            }
            if (j < n - 1) 
            {
                int right = idx + 1;
                unionSet(uf, idx * 4 + 1, right * 4 + 3);
            }
            if (grid[i][j] == '/') 
            {
                unionSet(uf, idx * 4, idx * 4 + 3);
                unionSet(uf, idx * 4 + 1, idx * 4 + 2);
            } 
            else if (grid[i][j] == '\\') 
            {
                unionSet(uf, idx * 4, idx * 4 + 1);
                unionSet(uf, idx * 4 + 2, idx * 4 + 3);
            } 
            else 
            {
                unionSet(uf, idx * 4, idx * 4 + 1);
                unionSet(uf, idx * 4 + 1, idx * 4 + 2);
                unionSet(uf, idx * 4 + 2, idx * 4 + 3);
            }
        }
    }

    struct HashTable* fathers = NULL;
    for (int i = 0; i < n * n * 4; i++) 
    {
        int fa = find(uf, i);
        struct HashTable* tmp;
        HASH_FIND_INT(fathers, &fa, tmp);
        if (tmp == NULL) 
        {
            tmp = malloc(sizeof(struct HashTable));
            tmp->val = fa;
            HASH_ADD_INT(fathers, val, tmp);
        }
    }
    return HASH_COUNT(fathers);
}