/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 13:42:51 by gansari           #+#    #+#             */
/*   Updated: 2025/06/03 19:01:11 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

int	g_signal_received = 0;

/**
* Signal handler for SIGINT (Ctrl+C) in interactive mode
*
* Handles the interrupt signal sent by pressing Ctrl+C. Instead of terminating
* the shell, this handler provides a clean user experience by:
* 1. Recording the signal receipt in g_signal_received
* 2. Printing a newline for clean output formatting
* 3. Using readline to clear the current line and redisplay the prompt
*
* This allows users to cancel current input without exiting the shell.
*
* @param sig The signal number (ignored as we know it's SIGINT)
*/
void	handle_sigint(int sig)
{
	(void)sig;
	g_signal_received = 1;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/**
* Signal handler for SIGINT during command execution
*
* Simpler version that just records the signal but doesn't redisplay prompt
*
* @param sig The signal number
*/
void	handle_sigint_exec(int sig)
{
	(void)sig;
	g_signal_received = 1;
	write(1, "\n", 1);
}

/**
* Signal handler for SIGQUIT
*
* This handler allows the shell to ignore SIGQUIT for itself but lets
* the signal pass to child processes.
*
* @param sig The signal number
*/
void	handle_sigquit_parent(int sig)
{
	(void)sig;
}

/**
* Configure signal handling based on shell's operational mode
*
* Dynamically adjusts signal behavior to provide appropriate
* handling for
* different states of shell operation:
*
* Mode 0 (Interactive mode - waiting for user input):
*   - SIGINT (Ctrl+C): Custom handler to clear input without exiting
*   - SIGQUIT (Ctrl+\): Ignored to prevent accidental termination
*
* Mode 1 (Command execution in child):
*   - SIGINT and SIGQUIT: Default behavior to allow process termination
*
* Mode 2 (Parent waiting for child):
*   - SIGINT: Custom handler that doesn't redisplay prompt
*   - SIGQUIT: Custom handler that lets signal pass to child but protects
* parent
*
* Mode 3 (Continuation input - waiting for additional input like
* unbalanced quotes):
*   - SIGINT (Ctrl+C): Custom handler to exit continuation with status
* 130
*   - SIGQUIT (Ctrl+\): Ignored to maintain continuation state
* (bash behavior)
*
* @param mode The current operational mode (0, 1, 2, or 3)
* @param shell The shell structure for accessing/updating exit status
*/
void	setup_signals(int mode, t_shell *shell)
{
	if (mode == 0)
	{
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
		if (g_signal_received)
		{
			shell->exit_status = 130;
			g_signal_received = 0;
		}
	}
	else if (mode == 1)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	else if (mode == 2)
	{
		signal(SIGINT, handle_sigint_exec);
		signal(SIGQUIT, handle_sigquit_parent);
	}
	else if (mode == 3)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
	}
}
