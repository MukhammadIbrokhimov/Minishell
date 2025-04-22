/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:53:29 by mukibrok          #+#    #+#             */
/*   Updated: 2025/04/22 12:54:51 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

void free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return;
	if (cmd->type == EXEC)
	{
		t_execcmd *ecmd = (t_execcmd *)cmd;
		for (int i = 0; ecmd->argv[i]; i++)
			free(ecmd->argv[i]);
	}
	else if (cmd->type == REDIR)
	{
		t_redircmd *rcmd = (t_redircmd *)cmd;
		free_cmd(rcmd->cmd);
		free(rcmd->file);
		free(rcmd->efile);
	}
	else if (cmd->type == PIPE)
	{
		t_pipecmd *pcmd = (t_pipecmd *)cmd;
		free_cmd(pcmd->left);
		free_cmd(pcmd->right);
	}
	else if (cmd->type == LIST)
	{
		t_listcmd *lcmd = (t_listcmd *)cmd;
		free_cmd(lcmd->left);
		free_cmd(lcmd->right);
	}
	else if (cmd->type == BACK)
	{
		t_backcmd *bcmd = (t_backcmd *)cmd;
		free_cmd(bcmd->cmd);
	}
	free(cmd);
}