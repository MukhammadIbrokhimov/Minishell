#include "../../includes/sadaf.h"

void ft_error(char *msg)
{
    ft_putstr_fd("sadaf: ", STDERR_FILENO);
    ft_putstr_fd(msg, STDERR_FILENO);
    ft_putstr_fd("\n", STDERR_FILENO);
}

void ft_perror(char *msg)
{
    ft_putstr_fd("sadaf: ", STDERR_FILENO);
    ft_putstr_fd(msg, STDERR_FILENO);
    ft_putstr_fd(": ", STDERR_FILENO);
    ft_putstr_fd(strerror(errno), STDERR_FILENO);
    ft_putstr_fd("\n", STDERR_FILENO);
}

int fork_safely(void)
{
    int pid;
    
    pid = fork();
    if (pid < 0)
    {
        ft_perror("fork");
        exit(1);
    }
    
    return (pid);
}

void *ft_calloc(size_t nmemb, size_t size)
{
    void *ptr;
    
    ptr = malloc(nmemb * size);
    if (!ptr)
        return (NULL);
    
    ft_bzero(ptr, nmemb * size);
    return (ptr);
}

void ft_bzero(void *s, size_t n)
{
    memset(s, 0, n);
}

int ft_isalpha(int c)
{
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

int ft_isalnum(int c)
{
    return (ft_isalpha(c) || ft_isdigit(c));
}

int ft_isdigit(int c)
{
    return (c >= '0' && c <= '9');
}

void expand_variables(t_execcmd *ecmd, t_shell *shell)
{
    int i;
    int j;
    char *expanded;
    char *var;
    char *dollar;
    
    for (i = 0; ecmd->argv[i]; i++)
    {
        dollar = strchr(ecmd->argv[i], '$');
        if (!dollar)
            continue;
        
        // Simple variable expansion, doesn't handle quotes or nested variables
        expanded = ft_strdup("");
        j = 0;
        
        while (ecmd->argv[i][j])
        {
            if (ecmd->argv[i][j] == '$')
            {
                // Handle special case $?
                if (ecmd->argv[i][j + 1] == '?')
                {
                    char exit_str[12];
                    snprintf(exit_str, sizeof(exit_str), "%d", shell->exit_status);
                    
                    char *tmp = expanded;
                    expanded = ft_strjoin(expanded, exit_str);
                    free(tmp);
                    j += 2;
                    continue;
                }
                
                // Extract variable name
                int start = j + 1;
                while (ecmd->argv[i][j + 1] && (ft_isalnum(ecmd->argv[i][j + 1]) || ecmd->argv[i][j + 1] == '_'))
                    j++;
                
                if (start <= j)
                {
                    var = ft_substr(ecmd->argv[i], start, j - start + 1);
                    char *value = ft_getenv(var, shell);
                    
                    char *tmp = expanded;
                    if (value)
                        expanded = ft_strjoin(expanded, value);
                    else
                        expanded = ft_strjoin(expanded, "");
                    
                    free(tmp);
                    free(var);
                }
            }
            else
            {
                char tmp_str[2] = {ecmd->argv[i][j], '\0'};
                char *tmp = expanded;
                expanded = ft_strjoin(expanded, tmp_str);
                free(tmp);
            }
            
            j++;
        }
        
        free(ecmd->argv[i]);
        ecmd->argv[i] = expanded;
    }
}

void cleanup_tokens(char **tokens)
{
    int i;
    
    if (!tokens)
        return;
    
    for (i = 0; tokens[i]; i++)
        free(tokens[i]);
    
    free(tokens);
}