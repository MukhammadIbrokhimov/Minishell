/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muxammad <muxammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 17:38:15 by muxammad          #+#    #+#             */
/*   Updated: 2025/04/15 17:40:58 by muxammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/sadaf.h"

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

t_shell *init_shell(char **envp)
{
    t_shell *shell;
    
    shell = (t_shell *)malloc(sizeof(t_shell));
    if (!shell)
        return (NULL);
    
    shell->env_list = parse_env(envp);
    shell->exit_status = 0;
    shell->in_heredoc = 0;
    
    return (shell);
}

int main(int argc, char **argv, char **envp)
{
    t_shell *shell;
    int exit_code;
    
    (void)argc;
    (void)argv;
    
    shell = init_shell(envp);
    if (!shell)
        return (1);
    
    //setup_signals(0);
    //exit_code = sadaf_prompt(shell);
    
    //free_shell(shell);
    return (exit_code);
}