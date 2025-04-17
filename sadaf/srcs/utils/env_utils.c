#include "../../includes/sadaf.h"

t_env *create_env_node(char *name, char *value)
{
    t_env *node;
    
    node = malloc(sizeof(t_env));
    if (!node)
        return (NULL);
    
    node->name = ft_strdup(name);
    node->value = ft_strdup(value);
    node->next = NULL;
    
    if (!node->name || !node->value)
    {
        free(node->name);
        free(node->value);
        free(node);
        return (NULL);
    }
    
    return (node);
}

void add_env_node(t_shell *shell, t_env *new_node)
{
    t_env *current;
    
    if (!shell || !new_node)
        return;
    
    if (!shell->env_list)
    {
        shell->env_list = new_node;
        return;
    }
    
    current = shell->env_list;
    while (current->next)
        current = current->next;
    
    current->next = new_node;
}

void free_env_list(t_env *env_list)
{
    t_env *temp;
    
    while (env_list)
    {
        temp = env_list;
        env_list = env_list->next;
        free(temp->name);
        free(temp->value);
        free(temp);
    }
}

/* Helper function to parse and create an environment node */
t_env *parse_and_create_env_node(char *env_str)
{
    char *equal_sign;
    char *name;
    char *value;
    t_env *new_node;

    equal_sign = strchr(env_str, '=');
    if (!equal_sign)
        return (NULL);
    
    name = ft_substr(env_str, 0, equal_sign - env_str);
    if (!name)
        return (NULL);
        
    value = ft_strdup(equal_sign + 1);
    if (!value) {
        free(name);
        return (NULL);
    }
    
    new_node = create_env_node(name, value);
    free(name);
    free(value);
    return (new_node);
}

t_env *parse_env(char **envp)
{
    t_env *env_list;
    t_env *new_node;
    int i;
    
    env_list = NULL;
    i = 0;
    while (envp[i])
    {
        new_node = parse_and_create_env_node(envp[i]);
        if (!new_node)
        {
            i++;
            continue;
        }
        if (!env_list)
            env_list = new_node;
        else
        {
            t_env *current = env_list;
            while (current->next)
                current = current->next;
            current->next = new_node;
        }
        i++;
    }
    return (env_list);
}

char **env_to_array(t_env *env_list)
{
    char **arr;
    t_env *current;
    int count;
    int i;
    
    count = 0;
    current = env_list;
    while (current)
    {
        count++;
        current = current->next;
    }
    
    arr = (char **)malloc(sizeof(char *) * (count + 1));
    if (!arr)
        return (NULL);
    
    i = 0;
    current = env_list;
    while (current)
    {
        char *temp1 = ft_strjoin(current->name, "=");
        if (!temp1)
        {
            cleanup_tokens(arr);
            return (NULL);
        }
        
        arr[i] = ft_strjoin(temp1, current->value);
        free(temp1);
        
        if (!arr[i])
        {
            cleanup_tokens(arr);
            return (NULL);
        }
        
        i++;
        current = current->next;
    }
    
    arr[i] = NULL;
    return (arr);
}

char *ft_getenv(char *name, t_shell *shell)
{
    t_env *current;
    
    current = shell->env_list;
    while (current)
    {
        if (ft_strcmp(current->name, name) == 0)
            return (current->value);
        
        current = current->next;
    }
    
    return (NULL);
}