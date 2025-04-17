#include "../../includes/sadaf.h"

char *ft_strdup(const char *s)
{
    size_t len;
    char *dup;
    
    len = strlen(s);
    dup = malloc(len + 1);
    if (!dup)
        return (NULL);
    
    memcpy(dup, s, len + 1);
    return (dup);
}

char *ft_strndup(const char *s, size_t n)
{
    size_t len;
    char *dup;
    
    len = 0;
    while (len < n && s[len])
        len++;
    
    dup = malloc(len + 1);
    if (!dup)
        return (NULL);
    
    memcpy(dup, s, len);
    dup[len] = '\0';
    return (dup);
}

int ft_strcmp(const char *s1, const char *s2)
{
    int i;
    
    i = 0;
    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int ft_strncmp(const char *s1, const char *s2, size_t n)
{
    size_t i;
    
    i = 0;
    while (i < n && s1[i] && s2[i])
    {
        if (s1[i] != s2[i])
            return ((unsigned char)s1[i] - (unsigned char)s2[i]);
        i++;
    }
    
    if (i < n)
        return ((unsigned char)s1[i] - (unsigned char)s2[i]);
    
    return (0);
}

char *ft_strjoin(char const *s1, char const *s2)
{
    size_t len1;
    size_t len2;
    char *joined;
    
    if (!s1 || !s2)
        return (NULL);
    
    len1 = strlen(s1);
    len2 = strlen(s2);
    
    joined = malloc(len1 + len2 + 1);
    if (!joined)
        return (NULL);
    
    memcpy(joined, s1, len1);
    memcpy(joined + len1, s2, len2 + 1);
    
    return (joined);
}

size_t ft_strlen(const char *s)
{
    size_t len;
    
    len = 0;
    while (s[len])
        len++;
    
    return (len);
}

char *ft_substr(char const *s, unsigned int start, size_t len)
{
    size_t s_len;
    char *sub;
    size_t i;
    
    if (!s)
        return (NULL);
    
    s_len = strlen(s);
    if (start >= s_len)
        return (ft_strdup(""));
    
    if (len > s_len - start)
        len = s_len - start;
    
    sub = malloc(len + 1);
    if (!sub)
        return (NULL);
    
    i = 0;
    while (i < len && s[start + i])
    {
        sub[i] = s[start + i];
        i++;
    }
    
    sub[i] = '\0';
    return (sub);
}

void ft_putstr_fd(char *s, int fd)
{
    if (!s)
        return;
    
    write(fd, s, strlen(s));
}

char **ft_split(char const *s, char c)
{
    char **arr;
    size_t count;
    size_t i;
    size_t j;
    
    if (!s)
        return (NULL);
    
    count = 0;
    i = 0;
    while (s[i])
    {
        if (s[i] != c && (i == 0 || s[i - 1] == c))
            count++;
        i++;
    }
    
    arr = malloc(sizeof(char *) * (count + 1));
    if (!arr)
        return (NULL);
    
    i = 0;
    j = 0;
    while (j < count)
    {
        while (s[i] == c)
            i++;
        
        size_t start = i;
        while (s[i] && s[i] != c)
            i++;
        
        arr[j] = ft_strndup(s + start, i - start);
        if (!arr[j])
        {
            cleanup_tokens(arr);
            return (NULL);
        }
        
        j++;
    }
    
    arr[count] = NULL;
    return (arr);
}


