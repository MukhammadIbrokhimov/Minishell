/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sadaf.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:38:55 by mukibrok          #+#    #+#             */
/*   Updated: 2025/04/24 18:05:48 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sadaf.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	char	*buf;

	(void)argc;
	(void)argv;
	shell = init_shell(envp);
	if (!shell)
		return (perror("Failed to initialize shell"), EXIT_FAILURE);
	setup_signals(0); // Set up signals for interactive mode
	while(1)
	{
		buf = getcmd();
		if (!buf) // Handle EOF (Ctrl+D)
			break;
		if(buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' ')
		{
			buf[strlen(buf)-1] = 0;
			if(chdir(buf+3) < 0)
				fprintf(stderr, "cannot cd %s\n", buf+3);
			free(buf);
			continue;
		}
		if(fork1() == 0)
		{
			t_cmd *cmd = parsecmd(buf);
			runcmd(cmd, shell);
			free_cmd(cmd);
			exit(EXIT_SUCCESS);
		}
		wait(NULL);
		free(buf);
	}
	free_shell(shell);
	return (EXIT_SUCCESS);
}