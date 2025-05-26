/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_heredoc_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:28:13 by gansari           #+#    #+#             */
/*   Updated: 2025/05/26 17:27:59 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

int	process_heredoc_in_redircmd(t_redircmd *rcmd, t_shell *shell)
{
	int		heredoc_fd;
	char	*delimiter;

	// CRITICAL FIX: Only process actual heredocs (<<), skip other redirections
	// This prevents early validation of regular redirections during parsing
	if (!rcmd->heredoc)
		return (0);  // Success - not a heredoc, skip it
	
	delimiter = ft_substr(rcmd->file, 0, rcmd->efile - rcmd->file);
	if (!delimiter)
		return (-1);
	heredoc_fd = handle_heredoc(delimiter, shell);
	free(delimiter);
	if (heredoc_fd < 0)
		return (-1);
	rcmd->heredoc = false;
	rcmd->fd = heredoc_fd;
	rcmd->file = NULL;
	rcmd->mode = O_RDONLY;
	return (0);
}

int	collect_heredocs_in_redir(t_cmd *cmd, t_shell *shell)
{
	t_redircmd	*rcmd;

	rcmd = (t_redircmd *)cmd;
	if (process_heredoc_in_redircmd(rcmd, shell) < 0)
		return (-1);
	return (collect_all_heredocs(rcmd->cmd, shell));
}

int	collect_heredocs_in_pipe(t_cmd *cmd, t_shell *shell)
{
	t_pipecmd	*pcmd;

	pcmd = (t_pipecmd *)cmd;
	if (collect_all_heredocs(pcmd->left, shell) < 0)
		return (-1);
	return (collect_all_heredocs(pcmd->right, shell));
}

int	collect_heredocs_in_list(t_cmd *cmd, t_shell *shell)
{
	t_listcmd	*lcmd;

	lcmd = (t_listcmd *)cmd;
	if (collect_all_heredocs(lcmd->left, shell) < 0)
		return (-1);
	return (collect_all_heredocs(lcmd->right, shell));
}

int	collect_heredocs_in_back(t_cmd *cmd, t_shell *shell)
{
	t_backcmd	*bcmd;

	bcmd = (t_backcmd *)cmd;
	return (collect_all_heredocs(bcmd->cmd, shell));
}