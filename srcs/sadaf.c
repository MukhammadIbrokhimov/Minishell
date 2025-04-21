/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sadaf.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muxammad <muxammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:38:55 by mukibrok          #+#    #+#             */
/*   Updated: 2025/04/21 13:53:32 by muxammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sadaf.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	char	*buf;
	int fd;

	while((fd = open("console", O_RDWR)) >= 0){
		if(fd >= 3){ // if we have more than stdin stdout and stderr close file descriptor
		close(fd);
		break;
		}
	}
	shell = init_shell(envp);
	if (!shell)
		return (perror("Failed to initialize shell"), EXIT_FAILURE);
	// Read and run input commands.
	while(1)
	{
		buf = getcmd();
		if(buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' ')
		{
			// Chdir must be called by the parent, not the child.
			buf[strlen(buf)-1] = 0;  // chop \n
			if(chdir(buf+3) < 0)
				fprintf(stderr, "cannot cd %s\n", buf+3);
			continue;
		}
		if(fork1() == 0)
			runcmd(parsecmd(buf), shell);
		wait(NULL);
	}
	free(buf);
	free_shell(shell);
	return (EXIT_SUCCESS);
}