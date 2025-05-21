/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sadaf.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 11:28:51 by gansari           #+#    #+#             */
/*   Updated: 2025/05/21 16:40:39 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sadaf.h"

static int	should_skip_command(char *buf)
{
	int	token_check;

	if (should_skip_empty_command(buf))
		return (1);
	if (handle_special_command(buf))
		return (3);
	token_check = if_only_token(buf);
	if (token_check)
		return (token_check);
	token_check = if_contains_lparen(buf);
	if (token_check)
		return (token_check);
	return (0);
}

void	shell_loop(t_shell *shell)
{
	char	*buf;
	int		skip_status;

	while (1)
	{
		prepare_for_command();
		buf = getcmd();
		if (!buf)
			break ;
		skip_status = should_skip_command(buf);
		if (skip_status)
		{
			free(buf);
			if (skip_status == 3)
				shell->exit_status = 1;
			if (skip_status == 2)
				shell->exit_status = 2;
			continue ;
		}
		execution(buf, shell);
		free(buf);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	(void)argc;
	(void)argv;
	shell = init_shell(envp);
	if (!shell)
		return (perror("Failed to initialize shell"), EXIT_FAILURE);
	setup_signals(0);
	shell_loop(shell);
	free_shell(shell);
	rl_clear_history();
	return (EXIT_SUCCESS);
}
