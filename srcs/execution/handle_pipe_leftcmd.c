/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe_leftcmd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:00:55 by gansari           #+#    #+#             */
/*   Updated: 2025/05/26 17:27:12 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * child_left_cmd - Handles execution in the left command's child process
 * 
 * CRITICAL FIX: In pipes, the left command should run and exit with its own status,
 * but this should NOT affect the pipe's overall exit status. The pipe continues
 * regardless of left command success/failure.
 */
static void	child_left_cmd(t_pipecmd *pcmd, t_shell *shell, int *fd)
{
	if (setup_pipe_output(fd) < 0)
		exit(EXIT_FAILURE);
	
	// FIXED: Let the command run and handle its own errors
	// If redirection fails, this process exits, but the pipe continues
	// The right side will get EOF and that's fine
	runcmd(pcmd->left, shell);
	
	// This should not be reached if runcmd works correctly
	exit(EXIT_FAILURE);
}

/**
 * execute_left_cmd - Creates and configures child process for the left command
 * 
 * The key insight: fork failures are different from command execution failures
 * - Fork failure: can't create the process at all -> pipe should fail
 * - Command execution failure: process runs but exits with error -> pipe continues
 */
int	execute_left_cmd(t_pipecmd *pcmd, t_shell *shell, int *fd)
{
	int	pid;

	pid = protected_fork();
	if (pid == 0)
		child_left_cmd(pcmd, shell, fd);
	else if (pid < 0)
	{
		ft_perror("fork");
		return (-1);
	}
	return (pid);
}