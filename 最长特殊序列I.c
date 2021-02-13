int findLUSlength(char * a, char * b)
{
    //return 0;
    if(strlen(a)==strlen(b))
    {
        if(!strcmp(a,b))
        {
            return -1;
        }
        else return strlen(a);
    }
    else
    {
        return strlen(a)>strlen(b)? strlen(a):strlen(b);
    }
}
