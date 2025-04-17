/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sadaf.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:38:55 by mukibrok          #+#    #+#             */
/*   Updated: 2025/04/17 17:17:08 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	static char buf[100];
	int fd;

	while((fd = open("console", O_RDWR)) >= 0){
		if(fd >= 3){ // if we have more than stdin stdout and stderr close file descriptor
		close(fd);
		break;
		}
	}
	shell = init_shell(NULL);
	if (!shell)
	{
		perror("Failed to initialize shell");
		return (EXIT_FAILURE);
	}
	// Read and run input commands.
	while(getcmd(buf, sizeof(buf)) >= 0)
	{
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
	free_shell(shell);
	return (EXIT_SUCCESS);
}