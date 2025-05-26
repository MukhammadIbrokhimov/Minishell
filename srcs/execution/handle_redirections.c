/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:23:16 by gansari           #+#    #+#             */
/*   Updated: 2025/05/26 17:26:11 by gansari          ###   ########.fr       */
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
	ft_putstr_fd(strerror(errno), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
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
 * collect_all_redirections - Collect redirections in order they appear (left to right)
 * @rcmd: Starting redirection command
 * @redirections: Array to store redirection pointers
 * @count: Pointer to count of redirections found
 * 
 * Returns: 0 on success, -1 on error
 */
static int collect_all_redirections(t_redircmd *rcmd, t_redircmd **redirections, int *count)
{
	t_cmd *current;
	int i = 0;
	
	current = (t_cmd *)rcmd;
	
	// Collect all redirections in a stack (reverse order)
	while (current && current->type == REDIR && i < 10) // max 10 redirections
	{
		redirections[i++] = (t_redircmd *)current;
		current = ((t_redircmd *)current)->cmd;
	}
	
	*count = i;
	return (0);
}

/*
 * validate_redirections_left_to_right - Validate redirections in left-to-right order
 * @redirections: Array of redirection pointers
 * @count: Number of redirections
 * 
 * Returns: 0 if all valid, -1 if any error
 */
static int validate_redirections_left_to_right(t_redircmd **redirections, int count)
{
	int i;
	char *filename;
	char *clean_filename;
	int fd;
	t_redircmd *redir;
	
	// Process from last to first (which is left to right in original command)
	for (i = count - 1; i >= 0; i--)
	{
		redir = redirections[i];
		
		// Skip heredocs and fd-based redirections
		if (!redir->file)
			continue;
		
		filename = ft_substr(redir->file, 0, redir->efile - redir->file);
		if (!filename)
			return (-1);
			
		clean_filename = remove_quotes(filename);
		if (!clean_filename)
		{
			free(filename);
			return (-1);
		}
		
		// Test if file can be opened
		if (redir->mode & O_RDONLY)
			fd = open(clean_filename, redir->mode);
		else
			fd = open(clean_filename, redir->mode, 0644);
			
		if (fd < 0)
		{
			print_error(clean_filename);
			free(filename);
			free(clean_filename);
			return (-1);
		}
		
		close(fd);
		free(filename);
		free(clean_filename);
	}
	
	return (0);
}

/*
 * validate_all_redirections - Check all redirections in the chain for errors
 * @rcmd: Redirection command to check
 * 
 * Returns: 0 if all valid, -1 if any redirection has an error
 * 
 * FIXED: This function now validates ALL redirections from left to right
 * and reports the FIRST error encountered, matching bash behavior
 */
static int validate_all_redirections(t_redircmd *rcmd)
{
	t_redircmd *redirections[10]; // max 10 redirections
	int count;
	
	if (collect_all_redirections(rcmd, redirections, &count) < 0)
		return (-1);
	
	return validate_redirections_left_to_right(redirections, count);
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
 * FIXED: Now validates ALL redirections first before applying any
 * This ensures we catch errors in the correct order (left to right)
 */
void	handle_redirections(t_redircmd *rcmd, t_shell *shell)
{
	int result;

	// FIXED: Validate all redirections first
	if (validate_all_redirections(rcmd) < 0)
		exit(1);

	// Apply the actual redirection (only the outermost one)
	if (rcmd->file)
		result = handle_file_redirection(rcmd);
	else
		result = handle_fd_redirection(rcmd);
		
	if (result < 0)
		exit(1);
		
	runcmd(rcmd->cmd, shell);
}