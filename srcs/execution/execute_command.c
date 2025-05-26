/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:22:03 by gansari           #+#    #+#             */
/*   Updated: 2025/05/26 19:05:53 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

static void	handle_builtin(t_execcmd *ecmd, t_shell *shell)
{
	int	exit_code;

	exit_code = exec_builtin(ecmd, shell);
	exit(exit_code);
}

static int	check_if_directory(char *path)
{
	struct stat	path_stat;

	if (!path || !*path)
		return (0);
	if (stat(path, &path_stat) == 0)
		return (S_ISDIR(path_stat.st_mode));
	return (0);
}

static void	command_not_found(char *cmd)
{
	if (!cmd)
		cmd = "(null)";
	ft_putstr_fd("\x1b[31msadaf: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	exit(127);
}

static void	handle_directory_error(char *cmd)
{
	if (!cmd)
		cmd = "(null)";
	ft_putstr_fd("\x1b[31msadaf: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": Is a directory\n", 2);
	exit(126);
}

static char	**prepare_unquoted_args(char **argv, char *path)
{
	char	**unquoted_argv;
	int		i;

	unquoted_argv = allocate_unquoted_array(argv, path);
	i = 0;
	while (argv[i])
	{
		unquoted_argv[i] = remove_quotes(argv[i]);
		if (!unquoted_argv[i])
			handle_unquote_error(unquoted_argv, path);
		i++;
	}
	unquoted_argv[i] = NULL;
	return (unquoted_argv);
}

static void	exec_external_command(char *path, char **argv, t_shell *shell)
{
	char	**env_array;
	char	**unquoted_argv;

	unquoted_argv = prepare_unquoted_args(argv, path);
	env_array = env_to_array(shell->env_list);
	if (!env_array)
	{
		free(path);
		cleanup_tokens(unquoted_argv);
		ft_perror("env_to_array");
		exit(1);
	}
	if (execve(path, unquoted_argv, env_array) < 0)
	{
		free(path);
		cleanup_tokens(unquoted_argv);
		cleanup_tokens(env_array);
		ft_perror("execve");
		exit(1);
	}
}

static void	try_execute_as_command(char *expanded_cmd, t_shell *shell)
{
	char	**tokens;
	char	*path;
	int		i;

	tokens = ft_split(expanded_cmd, ' ');
	if (!tokens || !tokens[0])
	{
		if (tokens)
			cleanup_tokens(tokens);
		command_not_found(expanded_cmd);
		return;
	}
	if (is_builtin(tokens[0]))
	{
		t_execcmd	cmd;
		int			status;

		ft_memset(&cmd, 0, sizeof(cmd));
		cmd.type = EXEC;
		i = 0;
		while (tokens[i] && i < MAXARGS - 1)
		{
			cmd.argv[i] = tokens[i];
			i++;
		}
		cmd.argv[i] = NULL;
		status = exec_builtin(&cmd, shell);
		cleanup_tokens(tokens);
		exit(status);
	}
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

static void	handle_empty_command(void)
{
	exit(0);
}

static int	is_empty_or_whitespace(char *str)
{
	int i;
	
	if (!str)
		return (1);
	
	i = 0;
	while (str[i])
	{
		if (!ft_isspace(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	execute_command(t_execcmd *ecmd, t_shell *shell)
{
	char	*path;
	char	*cmd_no_quotes;

	check_cmd_args(ecmd, shell);
	expand_variables(ecmd, shell);
	
	if (!ecmd->argv[0] || is_empty_or_whitespace(ecmd->argv[0]))
	{
		handle_empty_command();
		return;
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
		handle_empty_command();
		return;
	}
	if (ft_strchr(cmd_no_quotes, ' ') && 
		cmd_no_quotes[0] != '/' && 
		!(cmd_no_quotes[0] == '.' && cmd_no_quotes[1] == '/') &&
		!(cmd_no_quotes[0] == '.' && cmd_no_quotes[1] == '.' && cmd_no_quotes[2] == '/'))
	{
		try_execute_as_command(cmd_no_quotes, shell);
		free(cmd_no_quotes);
		return;
	}
	if (is_builtin(cmd_no_quotes))
	{
		free(cmd_no_quotes);
		handle_builtin(ecmd, shell);
		return ;
	}
	if (check_if_directory(cmd_no_quotes))
	{
		command_not_found(cmd_no_quotes);
		free(cmd_no_quotes);
		return;
	}
	path = find_command_path(cmd_no_quotes, shell);
	free(cmd_no_quotes);
	if (!path)
	{
		command_not_found(ecmd->argv[0]);
		return;
	}
	exec_external_command(path, ecmd->argv, shell);
	ft_error("execute_command: unreachable code");
	exit(1);
}
