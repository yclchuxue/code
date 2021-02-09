bool detectCapitalUse(char * word)
{
    int i, cnt = 0;
    int len = strlen(word);

    for (i = 0; i < len; i++)
        if(word[i] >= 'A' && word[i] <= 'Z')
            cnt++;

    if (cnt == len || cnt == 0 || cnt == 1 && *word >= 'A' && *word <= 'Z')
        return true;

    return false;
}