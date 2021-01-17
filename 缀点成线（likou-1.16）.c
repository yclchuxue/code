int checkStraightLine(int** coordinates, int coordinatesSize, int*coordinatesColSize)
{

    int A = coordinates[0][0]-coordinates[1][0];//x
    int B = coordinates[0][1]-coordinates[1][1];//y
    for(int i = 2;i<coordinatesSize;i++)
    {
        int x = coordinates[i][0]-coordinates[0][0];
        int y = coordinates[i][1]-coordinates[0][1];
        if(B*x != A*y)
        {
            return 0;
        }
    }
    return 1;
}
