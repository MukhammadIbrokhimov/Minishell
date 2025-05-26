/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseredir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:57:14 by muxammad          #+#    #+#             */
/*   Updated: 2025/05/26 15:15:09 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * create_redirection - Creates a redirection command
 * @cmd: Original command structure
 * @op_tok: Operator token (e.g., <, >, >>)
 * @file_tok: Filename token
 * @heredoc_flag: Flag indicating if it's a heredoc
 */

/**
 * fill_redirinfo - Fills redirection information based on operator token
 * @info: Pointer to redirection info structure
 * @op_tok: Operator token (e.g., <, >, >>)
 * @heredoc_flag: Flag indicating if it's a heredoc
 */

static void	fill_redirinfo(
	t_redirinfo *info, t_token op_tok, bool *heredoc_flag)
{
	if (op_tok.type == TOK_LT)
	{
		info->mode = O_RDONLY;
		info->fd = 0;
	}
	else if (op_tok.type == TOK_GT)
	{
		info->mode = O_WRONLY | O_CREAT | O_TRUNC;
		info->fd = 1;
	}
	else if (op_tok.type == TOK_DGT)
	{
		info->mode = O_WRONLY | O_CREAT | O_APPEND;
		info->fd = 1;
	}
	else if (op_tok.type == TOK_DLT)
	{
		info->mode = O_RDONLY;
		info->fd = 0;
		info->heredoc = true;
		*heredoc_flag = true;
	}
}

/**
 * create_redirection - Creates a redirection command
 * @cmd: Original command structure
 * @op_tok: Operator token (e.g., <, >, >>)
 * @file_tok: Filename token
 * @heredoc_flag: Flag indicating if it's a heredoc
 *
 * Returns: New command with redirection or NULL on failure
 *
 * This function creates a new command structure for redirection,
 * filling in the necessary information based on the operator and filename.
 */

t_cmd	*create_redirection(
	t_cmd *cmd, t_token op_tok, t_token file_tok, bool *heredoc_flag)
{
	t_redirinfo	info;

	*heredoc_flag = false;
	ft_memset(&info, 0, sizeof(info));
	fill_redirinfo(&info, op_tok, heredoc_flag);
	info.file = file_tok.start;
	info.efile = file_tok.end;
	return (redircmd(cmd, info));
}

/**
 * should_override_redirection - Check if new redirection should override existing ones
 * @new_fd: File descriptor of the new redirection
 * @existing_cmd: Existing command to check
 *
 * Returns: 1 if should override, 0 otherwise
 *
 * For input redirections (fd 0), the last one should take precedence.
 * This function checks if we need to override an existing input redirection.
 */
static int should_override_redirection(int new_fd, t_cmd *existing_cmd)
{
	t_redircmd *rcmd;
	
	if (!existing_cmd || existing_cmd->type != REDIR)
		return (0);
	
	rcmd = (t_redircmd *)existing_cmd;
	
	// If both are input redirections (fd 0), the new one should override
	return (new_fd == 0 && rcmd->fd == 0);
}

/**
 * override_input_redirection - Override existing input redirection with new one
 * @cmd: Command structure with existing redirection
 * @new_redir: New redirection command to use instead
 *
 * Returns: Modified command structure
 *
 * This function replaces an existing input redirection with a new one,
 * implementing bash's behavior where the last input redirection takes precedence.
 */
static t_cmd *override_input_redirection(t_cmd *cmd, t_cmd *new_redir)
{
	t_redircmd *old_rcmd;
	t_redircmd *new_rcmd;
	
	if (!cmd || cmd->type != REDIR || !new_redir || new_redir->type != REDIR)
		return (new_redir);
	
	old_rcmd = (t_redircmd *)cmd;
	new_rcmd = (t_redircmd *)new_redir;
	
	// Replace the old redirection's inner command with the new redirection's inner command
	// This preserves the command hierarchy while using the new redirection
	new_rcmd->cmd = old_rcmd->cmd;
	
	// Free the old redirection structure (but not its inner command, which we just moved)
	old_rcmd->cmd = NULL; // Prevent double-free
	free_cmd(cmd);
	
	return (new_redir);
}

/**
 * validate_redirection_file - Validates that a redirection file can be opened
 * @file_tok: Token containing the filename
 * @mode: File mode flags (O_RDONLY, O_WRONLY, etc.)
 * 
 * Returns: 0 on success, -1 on failure
 */
static int	validate_redirection_file(t_token file_tok, int mode)
{
	char	*filename;
	char	*clean_filename;
	int		fd;
	int		result;

	result = 0;
	filename = ft_substr(file_tok.start, 0, file_tok.end - file_tok.start);
	if (!filename)
		return (-1);
	clean_filename = remove_quotes(filename);
	if (!clean_filename)
	{
		free(filename);
		return (-1);
	}
	if (mode & O_RDONLY)
		fd = open(clean_filename, mode);
	else
		fd = open(clean_filename, mode, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("\x1b[31msadaf: ", STDERR_FILENO);
		ft_putstr_fd(clean_filename, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		result = -1;
	}
	else
		close(fd);
	free(filename);
	free(clean_filename);
	return (result);
}

/**
 * parseredirs - Parses redirections in command line
 * @cmd: Command structure to fill
 * @ps: Parser state to track position
 *
 * Returns: Command structure with redirections attached
 *
 * Modified to validate ALL redirections before processing overrides.
 * This ensures error messages are shown for all invalid files.
 */
t_cmd	*parseredirs(t_cmd *cmd, t_parserState *ps)
{
	t_token	op_tok;
	t_token	file_tok;
	bool	heredoc;
	t_cmd	*new_redir;
	int		mode;

	heredoc = false;
	while (1)
	{
		op_tok = gettoken(ps);
		if (op_tok.type != TOK_LT && op_tok.type != TOK_GT
			&& op_tok.type != TOK_DGT && op_tok.type != TOK_DLT)
		{
			ps->s = op_tok.start;
			break;
		}
		file_tok = gettoken(ps);
		if (file_tok.type != TOK_WORD)
			ft_exit("\x1b[31mSyntax error: Expected filename after redirection\n");
		if (op_tok.type == TOK_LT)
			mode = O_RDONLY;
		else if (op_tok.type == TOK_GT)
			mode = O_WRONLY | O_CREAT | O_TRUNC;
		else if (op_tok.type == TOK_DGT)
			mode = O_WRONLY | O_CREAT | O_APPEND;
		else if (op_tok.type == TOK_DLT)
			mode = O_RDONLY;
		if (op_tok.type != TOK_DLT)
		{
			if (validate_redirection_file(file_tok, mode) < 0)
				exit(1);
		}
		new_redir = create_redirection(cmd, op_tok, file_tok, &heredoc);
		if (!new_redir)
			ft_exit("Error: Failed to create redirection command\n");
		if (should_override_redirection(((t_redircmd *)new_redir)->fd, cmd))
			cmd = override_input_redirection(cmd, new_redir);
		else
			cmd = new_redir;
	}
	return (cmd);
}

