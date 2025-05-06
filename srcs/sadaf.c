/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sadaf.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:38:55 by mukibrok          #+#    #+#             */
/*   Updated: 2025/05/06 14:32:31 by mukibrok         ###   ########.fr       */
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
		if (strncmp(buf, "cd ", 3) == 0 || strcmp(buf, "cd") == 0) {
			// Strip newline
			buf[strcspn(buf, "\n")] = 0;
		
			char *path = buf + 2;
			while (*path == ' ') path++; // Skip all spaces after 'cd'
		
			if (*path == 0) {
				// No path specified, go to HOME
				path = getenv("HOME");
				if (!path) path = "/";
			}
		
			if (chdir(path) < 0)
				fprintf(stderr, "cannot cd to '%s'\n", path);
		
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