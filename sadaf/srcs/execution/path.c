#include "../../includes/sadaf.h"

char *find_command_path(char *cmd, t_shell *shell)
{
    char **paths;
    char *path;
    int i;
    
    if (!cmd || !*cmd)
        return (NULL);
    
    // If command contains a slash, use it directly
    if (strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return (ft_strdup(cmd));
        return (NULL);
    }
    
    paths = parse_path(shell->env_list);
    if (!paths)
        return (NULL);
    
    for (i = 0; paths[i]; i++)
    {
        path = build_path(paths[i], cmd);
        if (!path)
            continue;
        
        if (access(path, X_OK) == 0)
        {
            cleanup_tokens(paths);
            return (path);
        }
        
        free(path);
    }
    
    cleanup_tokens(paths);
    return (NULL);
}

char **parse_path(t_env *env_list)
{
    t_env *current;
    char **paths;
    
    current = env_list;
    while (current)
    {
        if (ft_strcmp(current->name, "PATH") == 0)
            break;
        current = current->next;
    }
    
    if (!current)
        return (NULL);
    
    paths = ft_split(current->value, ':');
    return (paths);
}

char *build_path(char *dir, char *cmd)
{
    char *path;
    char *tmp;
    
    if (!dir || !cmd)
        return (NULL);
    
    // Add trailing slash to directory if needed
    if (dir[strlen(dir) - 1] == '/')
        path = ft_strjoin(dir, cmd);
    else
    {
        tmp = ft_strjoin(dir, "/");
        if (!tmp)
            return (NULL);
        
        path = ft_strjoin(tmp, cmd);
        free(tmp);
    }
    
    return (path);
}