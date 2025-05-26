/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:23:16 by gansari           #+#    #+#             */
/*   Updated: 2025/05/26 15:18:57 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/*
 * open_file - Opens a file with specified access mode and proper error handling
 */
static int	open_file(char *file, int mode)
{
	int	fd;

	if (mode & O_RDONLY)
		fd = open(file, mode);
	else
		fd = open(file, mode, 0644);
	return (fd);
}

/*
 * print_error - Prints a formatted error message for file operations
 */
static void	print_error(char *file)
{
	ft_putstr_fd("\x1b[31msadaf: ", STDERR_FILENO);
	ft_putstr_fd(file, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_perror("");
}

/*
 * setup_file_redirection - Sets up file descriptor redirection
 * @fd: Source file descriptor
 * @target_fd: Target file descriptor (0 for stdin, 1 for stdout)
 * @file: Filename for error messages
 * 
 * Returns: 0 on success, -1 on failure
 */
static int	setup_file_redirection(int fd, int target_fd, char *file)
{
	if (fd < 0)
	{
		print_error(file);
		return (-1);
	}
	
	if (dup2(fd, target_fd) == -1)
	{
		ft_perror("sadaf: dup2");
		close(fd);
		return (-1);
	}
	
	close(fd);
	return (0);
}

/*
 * handle_file_redirection - Implements file redirection for shell commands
 * 
 * This function handles both input and output redirections:
 * - Input redirection (<): Opens file and redirects to stdin
 * - Output redirection (>, >>): Opens file and redirects to stdout
 */
static int	handle_file_redirection(t_redircmd *rcmd)
{
	int		fd;
	char	*filename;
	char	*clean_filename;

	filename = ft_substr(rcmd->file, 0, rcmd->efile - rcmd->file);
	if (!filename)
	{
		ft_putstr_fd("\x1b[31msadaf: memory allocation error\n", STDERR_FILENO);
		return (-1);
	}
	clean_filename = remove_quotes(filename);
	if (!clean_filename)
	{
		free(filename);
		ft_putstr_fd("\x1b[31msadaf: memory allocation error\n", STDERR_FILENO);
		return (-1);
	}
	fd = open_file(clean_filename, rcmd->mode);
	if (setup_file_redirection(fd, rcmd->fd, clean_filename) < 0)
	{
		free(filename);
		free(clean_filename);
		return (-1);
	}
	free(filename);
	free(clean_filename);
	return (0);
}

/*
 * handle_fd_redirection - Redirects using an already-open file descriptor
 * (Used primarily for heredocs)
 */
static int	handle_fd_redirection(t_redircmd *rcmd)
{
	if (dup2(rcmd->fd, STDIN_FILENO) == -1)
	{
		ft_perror("sadaf: dup2");
		close(rcmd->fd);
		return (-1);
	}
	close(rcmd->fd);
	return (0);
}

/*
 * handle_redirections - Main entry point for all redirection operations
 * 
 * This function handles all types of redirections and then executes the command.
 * The key fix: we continue execution even if redirection setup succeeds,
 * because the command should run with the redirections in place.
 */
void	handle_redirections(t_redircmd *rcmd, t_shell *shell)
{
	int result;

	if (rcmd->file)
		result = handle_file_redirection(rcmd);
	else
		result = handle_fd_redirection(rcmd);
	if (result < 0)
		exit(1);
	runcmd(rcmd->cmd, shell);
}
