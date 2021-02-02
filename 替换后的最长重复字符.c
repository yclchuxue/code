int characterReplacement(char* s, int k) 
{
    int num[26];
    memset(num, 0, sizeof(num));
    int n = strlen(s);
    int maxn = 0;
    int left = 0, right = 0;
    while (right < n) 
    {
        num[s[right] - 'A']++;
        maxn = fmax(maxn, num[s[right] - 'A']);
        if (right - left + 1 - maxn > k) 
        {
            num[s[left] - 'A']--;
            left++;
        }
        right++;
    }
    return right - left;
}