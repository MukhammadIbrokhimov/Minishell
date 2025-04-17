/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_envp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muxammad <muxammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:25:07 by muxammad          #+#    #+#             */
/*   Updated: 2025/04/17 11:25:08 by muxammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

void append_env_node(t_env **head, t_env **tail, t_env *new_node)
{
	if (!*head)
	{
		*head = new_node;
		*tail = new_node;
	}
	else
	{
		(*tail)->next = new_node;
		*tail = new_node;
	}
}

t_env	*create_node(char *envp)
{
	
}

t_env	*init_envp(char **envp)
{
	t_env	*new_node;
	t_env	*head;
	t_env	*tail;
	int		i;

	new_node = NULL;
	head = NULL;
	tail = NULL;
	i = 0;
	while (envp[i])
	{
		new_node = create_node(envp[i]);
		if (new_node)
			append_env_node(&head, &tail, new_node);
		i++;
	}
	return (head);
}

t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->env_list = init_envp(envp);
	shell->exit_status = 0;
	shell->in_heredoc = 0;
}
