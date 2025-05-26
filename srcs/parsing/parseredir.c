/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseredir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:57:14 by muxammad          #+#    #+#             */
/*   Updated: 2025/05/26 17:25:48 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

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
 * For input redirections (fd 0), the rightmost one should take precedence.
 */
static int should_override_redirection(int new_fd, t_cmd *existing_cmd)
{
	t_redircmd *rcmd;
	
	if (!existing_cmd || existing_cmd->type != REDIR)
		return (0);
	
	rcmd = (t_redircmd *)existing_cmd;
	
	// If both are input redirections (fd 0), the rightmost one should override
	return (new_fd == 0 && rcmd->fd == 0);
}

/**
 * override_input_redirection - Override existing input redirection with new one
 * @cmd: Command structure with existing redirection
 * @new_redir: New redirection command to use instead
 *
 * Returns: Modified command structure
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
	new_rcmd->cmd = old_rcmd->cmd;
	
	// Free the old redirection structure (but not its inner command, which we just moved)
	old_rcmd->cmd = NULL; // Prevent double-free
	free_cmd(cmd);
	
	return (new_redir);
}

/**
 * parseredirs - Parses redirections in command line
 * @cmd: Command structure to fill
 * @ps: Parser state to track position
 *
 * Returns: Command structure with redirections attached
 *
 * FIXED: Removed file validation during parsing - files should be checked at execution time
 * This allows pipes to work correctly even when left command has invalid redirections
 */
t_cmd	*parseredirs(t_cmd *cmd, t_parserState *ps)
{
	t_token	op_tok;
	t_token	file_tok;
	bool	heredoc;
	t_cmd	*new_redir;

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
		
		// REMOVED: File validation during parsing
		// Files will be validated during execution instead
		
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