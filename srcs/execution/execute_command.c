/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:22:03 by gansari           #+#    #+#             */
/*   Updated: 2025/05/26 20:48:31 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

static void	handle_builtin(t_execcmd *ecmd, t_shell *shell)
{
	int	exit_code;

	exit_code = exec_builtin(ecmd, shell);
	exit(exit_code);
}

static void	handle_external_tokens(char **tokens, t_shell *shell)
{
	char	*path;

	path = find_command_path(tokens[0], shell);
	if (!path)
	{
		if (check_if_directory(tokens[0]))
		{
			cleanup_tokens(tokens);
			handle_directory_error(tokens[0]);
		}
		cleanup_tokens(tokens);
		command_not_found(tokens[0]);
	}
	exec_external_command(path, tokens, shell);
}

static void	try_execute_as_command(char *expanded_cmd, t_shell *shell)
{
	char	**tokens;

	tokens = ft_split(expanded_cmd, ' ');
	if (!tokens || !tokens[0])
	{
		if (tokens)
			cleanup_tokens(tokens);
		command_not_found(expanded_cmd);
		return ;
	}
	if (is_builtin(tokens[0]))
	{
		handle_builtin_tokens(tokens, shell);
		return ;
	}
	handle_external_tokens(tokens, shell);
}

static void	handle_command_execution(char *cmd_no_quotes,
	t_execcmd *ecmd, t_shell *shell)
{
	char	*path;

	if (is_complex_command(cmd_no_quotes))
	{
		try_execute_as_command(cmd_no_quotes, shell);
		return ;
	}
	if (is_builtin(cmd_no_quotes))
	{
		handle_builtin(ecmd, shell);
		return ;
	}
	if (check_if_directory(cmd_no_quotes))
	{
		command_not_found(cmd_no_quotes);
		return ;
	}
	path = find_command_path(cmd_no_quotes, shell);
	if (!path)
	{
		command_not_found(ecmd->argv[0]);
		return ;
	}
	exec_external_command(path, ecmd->argv, shell);
}

void	execute_command(t_execcmd *ecmd, t_shell *shell)
{
	char	*cmd_no_quotes;

	check_cmd_args(ecmd, shell);
	expand_variables(ecmd, shell);
	if (!ecmd->argv[0] || is_empty_or_whitespace(ecmd->argv[0]))
	{
		exit(0);
		return ;
	}
	cmd_no_quotes = remove_quotes(ecmd->argv[0]);
	if (!cmd_no_quotes)
	{
		ft_error("remove_quotes failed");
		exit(1);
	}
	if (is_empty_or_whitespace(cmd_no_quotes))
	{
		free(cmd_no_quotes);
		exit(0);
		return ;
	}
	handle_command_execution(cmd_no_quotes, ecmd, shell);
	free(cmd_no_quotes);
	ft_error("execute_command: unreachable code");
	exit(1);
}
