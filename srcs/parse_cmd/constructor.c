/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 16:31:17 by muxammad          #+#    #+#             */
/*   Updated: 2025/04/22 14:57:21 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

t_cmd	*execcmd(void)
{
	t_execcmd	*cmd;

	cmd = ft_calloc(sizeof(*cmd), sizeof(*cmd));
	if (!cmd)
		return (fprintf(stderr,"execcmd: ft_calloc failed"), NULL);
	cmd->type = EXEC;
	return ((t_cmd *)cmd);
}

t_cmd	*redircmd(t_cmd *subcmd, char *file, char *efile, int mode, int fd)
{
	t_redircmd	*cmd;

	cmd = ft_calloc(sizeof(*cmd), sizeof(*cmd));
	if (!cmd)
	{
		fprintf(stderr,"redircmd: ft_calloc failed");
		return (free_cmd(cmd), NULL);
	}
	cmd->type = REDIR;
	cmd->cmd = subcmd;
	cmd->file = file;
	cmd->efile = efile;
	cmd->mode = mode;
	cmd->fd = fd;
	return ((t_cmd *)cmd);
}

t_cmd	*pipecmd(t_cmd *left, t_cmd *right)
{
	t_pipecmd	*cmd;

	cmd = ft_calloc(sizeof(*cmd), sizeof(*cmd));
	if (!cmd)
	{
		free_cmd(left);
		free_cmd(right);
		return (fprintf(stderr, "pipecmd: ft_calloc failed"), NULL);
	}
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	return ((t_cmd *)cmd);
}

t_cmd	*listcmd(t_cmd *left, t_cmd *right)
{
	t_listcmd	*cmd;

	cmd = ft_calloc(sizeof(*cmd), sizeof(*cmd));
	if (!cmd)
	{
		free_cmd(left);
		free_cmd(right);
		return (fprintf(stderr, "listcmd: ft_calloc failed"), NULL);
	}
	cmd->type = LIST;
	cmd->left = left;
	cmd->right = right;
	return ((t_cmd *)cmd);
}

t_cmd	*backcmd(t_cmd *subcmd)
{
	t_backcmd	*cmd;

	cmd = ft_calloc(sizeof(*cmd), sizeof(*cmd));
	if (!cmd)
	{
		free_cmd(subcmd);
		return (fprintf(stderr, "backcmd: ft_calloc failed"), NULL);
	}
	cmd->type = BACK;
	cmd->cmd = subcmd;
	return ((t_cmd *)cmd);
}
