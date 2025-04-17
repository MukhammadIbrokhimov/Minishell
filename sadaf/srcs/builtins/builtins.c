#include "../../includes/sadaf.h"

int is_builtin(char *cmd)
{
    if (!cmd)
        return (0);
    
    return (ft_strcmp(cmd, "echo") == 0 ||
            ft_strcmp(cmd, "cd") == 0 ||
            ft_strcmp(cmd, "pwd") == 0 ||
            ft_strcmp(cmd, "export") == 0 ||
            ft_strcmp(cmd, "unset") == 0 ||
            ft_strcmp(cmd, "env") == 0 ||
            ft_strcmp(cmd, "exit") == 0);
}

int exec_builtin(t_execcmd *ecmd, t_shell *shell)
{
    char *cmd;
    
    cmd = ecmd->argv[0];
    if (!cmd)
        return (1);
    
    if (ft_strcmp(cmd, "echo") == 0)
        return (builtin_echo(ecmd, shell));
    if (ft_strcmp(cmd, "cd") == 0)
        return (builtin_cd(ecmd, shell));
    if (ft_strcmp(cmd, "pwd") == 0)
        return (builtin_pwd(ecmd, shell));
    if (ft_strcmp(cmd, "export") == 0)
        return (builtin_export(ecmd, shell));
    if (ft_strcmp(cmd, "unset") == 0)
        return (builtin_unset(ecmd, shell));
    if (ft_strcmp(cmd, "env") == 0)
        return (builtin_env(ecmd, shell));
    if (ft_strcmp(cmd, "exit") == 0)
        return (builtin_exit(ecmd, shell));
    
    return (1);
}

int builtin_echo(t_execcmd *ecmd, t_shell *shell)
{
    int i;
    int n_flag;
    
    (void)shell;
    n_flag = 0;
    i = 1;
    
    if (ecmd->argv[1] && ft_strcmp(ecmd->argv[1], "-n") == 0)
    {
        n_flag = 1;
        i = 2;
    }
    
    while (ecmd->argv[i])
    {
        ft_putstr_fd(ecmd->argv[i], STDOUT_FILENO);
        if (ecmd->argv[i + 1])
            ft_putstr_fd(" ", STDOUT_FILENO);
        i++;
    }
    
    if (!n_flag)
        ft_putstr_fd("\n", STDOUT_FILENO);
    
    return (0);
}

int builtin_cd(t_execcmd *ecmd, t_shell *shell)
{
    char *path;
    char *old_pwd;
    char *current_pwd;
    char buf[1024];
    
    if (!ecmd->argv[1])
        path = ft_getenv("HOME", shell);
    else
        path = ecmd->argv[1];
    
    if (!path)
    {
        ft_putstr_fd("sadaf: cd: HOME not set\n", STDERR_FILENO);
        return (1);
    }
    
    old_pwd = getcwd(buf, sizeof(buf));
    if (!old_pwd)
    {
        ft_perror("cd");
        return (1);
    }
    
    if (chdir(path) != 0)
    {
        ft_putstr_fd("sadaf: cd: ", STDERR_FILENO);
        ft_putstr_fd(path, STDERR_FILENO);
        ft_putstr_fd(": ", STDERR_FILENO);
        ft_perror("");
        return (1);
    }
    
    current_pwd = getcwd(buf, sizeof(buf));
    if (!current_pwd)
    {
        ft_perror("cd");
        return (1);
    }
    
    // Update PWD and OLDPWD environment variables
    t_env *current = shell->env_list;
    while (current)
    {
        if (ft_strcmp(current->name, "PWD") == 0)
        {
            free(current->value);
            current->value = ft_strdup(current_pwd);
        }
        else if (ft_strcmp(current->name, "OLDPWD") == 0)
        {
            free(current->value);
            current->value = ft_strdup(old_pwd);
        }
        current = current->next;
    }
    
    return (0);
}

int builtin_pwd(t_execcmd *ecmd, t_shell *shell)
{
    char buf[1024];
    
    (void)ecmd;
    (void)shell;
    
    if (getcwd(buf, sizeof(buf)) == NULL)
    {
        ft_perror("pwd");
        return (1);
    }
    
    ft_putstr_fd(buf, STDOUT_FILENO);
    ft_putstr_fd("\n", STDOUT_FILENO);
    
    return (0);
}

int builtin_export(t_execcmd *ecmd, t_shell *shell)
{
    int i;
    char *equal_sign;
    char *name;
    char *value;
    t_env *current;
    
    if (!ecmd->argv[1])
    {
        // Print all environment variables in sorted order
        // For simplicity, we'll just print them unsorted here
        current = shell->env_list;
        while (current)
        {
            ft_putstr_fd("declare -x ", STDOUT_FILENO);
            ft_putstr_fd(current->name, STDOUT_FILENO);
            ft_putstr_fd("=\"", STDOUT_FILENO);
            ft_putstr_fd(current->value, STDOUT_FILENO);
            ft_putstr_fd("\"\n", STDOUT_FILENO);
            current = current->next;
        }
        return (0);
    }
    
    for (i = 1; ecmd->argv[i]; i++)
    {
        equal_sign = strchr(ecmd->argv[i], '=');
        if (!equal_sign)
        {
            // No value provided, just declare the variable
            name = ft_strdup(ecmd->argv[i]);
            value = ft_strdup("");
        }
        else
        {
            name = ft_substr(ecmd->argv[i], 0, equal_sign - ecmd->argv[i]);
            value = ft_strdup(equal_sign + 1);
        }
        
        // Check if variable already exists
        int found = 0;
        current = shell->env_list;
        while (current)
        {
            if (ft_strcmp(current->name, name) == 0)
            {
                free(current->value);
                current->value = ft_strdup(value);
                found = 1;
                break;
            }
            current = current->next;
        }
        
        // If not found, add new variable
        if (!found)
        {
            t_env *new_node = create_env_node(name, value);
            if (new_node)
                add_env_node(shell, new_node);
        }
        
        free(name);
        free(value);
    }
    
    return (0);
}

int builtin_unset(t_execcmd *ecmd, t_shell *shell)
{
    int i;
    t_env *current;
    t_env *prev;
    
    for (i = 1; ecmd->argv[i]; i++)
    {
        prev = NULL;
        current = shell->env_list;
        
        while (current)
        {
            if (ft_strcmp(current->name, ecmd->argv[i]) == 0)
            {
                if (!prev)
                    shell->env_list = current->next;
                else
                    prev->next = current->next;
                
                free(current->name);
                free(current->value);
                free(current);
                break;
            }
            
            prev = current;
            current = current->next;
        }
    }
    
    return (0);
}

int builtin_env(t_execcmd *ecmd, t_shell *shell)
{
    t_env *current;
    
    (void)ecmd;
    
    current = shell->env_list;
    while (current)
    {
        ft_putstr_fd(current->name, STDOUT_FILENO);
        ft_putstr_fd("=", STDOUT_FILENO);
        ft_putstr_fd(current->value, STDOUT_FILENO);
        ft_putstr_fd("\n", STDOUT_FILENO);
        current = current->next;
    }
    
    return (0);
}

int builtin_exit(t_execcmd *ecmd, t_shell *shell)
{
    int exit_code;
    
    ft_putstr_fd("exit\n", STDOUT_FILENO);
    
    if (!ecmd->argv[1])
        exit_code = shell->exit_status;
    else
    {
        // Check if argument is numeric
        int i = 0;
        while (ecmd->argv[1][i])
        {
            if (!ft_isdigit(ecmd->argv[1][i]))
            {
                ft_putstr_fd("sadaf: exit: ", STDERR_FILENO);
                ft_putstr_fd(ecmd->argv[1], STDERR_FILENO);
                ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
                exit_code = 2;
                break;
            }
            i++;
        }
        
        if (ecmd->argv[1][i] == '\0')
        {
            exit_code = atoi(ecmd->argv[1]);
            if (ecmd->argv[2])
            {
                ft_putstr_fd("sadaf: exit: too many arguments\n", STDERR_FILENO);
                return (1);
            }
        }
    }
    
    exit(exit_code & 0xFF);
}