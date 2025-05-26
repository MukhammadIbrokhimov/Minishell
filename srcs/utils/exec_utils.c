/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 15:35:27 by gansari           #+#    #+#             */
/*   Updated: 2025/05/26 15:17:01 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

static int	is_builtin_no_fork(const char *cmd)
{
	return (ft_strcmp((char *)cmd, "cd") == 0
		|| ft_strcmp((char *)cmd, "exit") == 0
		|| ft_strcmp((char *)cmd, "export") == 0
		|| ft_strcmp((char *)cmd, "unset") == 0);
}

static int	should_fork(t_cmd *cmd)
{
	t_execcmd	*ecmd;

	if (cmd->type != EXEC)
		return (1);
	ecmd = (t_execcmd *)cmd;
	return (!is_builtin_no_fork(ecmd->argv[0]));
}

static void	execute_builtin(t_cmd *cmd, t_shell *shell)
{
	t_execcmd	*ecmd;
	int			status;

	ecmd = (t_execcmd *)cmd;
	expand_variables(ecmd, shell);
	status = exec_builtin(ecmd, shell);
	shell->exit_status = status;
}

static void	execute_forked(t_cmd *cmd, t_shell *shell)
{
	int	status;
	int	pid;

	pid = protected_fork();
	if (pid == 0)
	{
		setup_signals(1, shell);
		runcmd(cmd, shell);
		free_cmd(cmd);
		exit(EXIT_SUCCESS);
	}
	setup_signals(2, shell);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		shell->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
	}
	setup_signals(0, shell);
}

// void debug_print_cmd(t_cmd *cmd, int depth) {
//     if (!cmd) return;
    
//     for (int i = 0; i < depth; i++) printf("  ");
    
//     if (cmd->type == EXEC) {
//         t_execcmd *ecmd = (t_execcmd *)cmd;
//         printf("EXEC: ");
//         for (int i = 0; ecmd->argv[i]; i++) {
//             printf("[%s] ", ecmd->argv[i]);
//         }
//         printf("\n");
//     }
//     else if (cmd->type == REDIR) {
//         t_redircmd *rcmd = (t_redircmd *)cmd;
//         char *filename = ft_substr(rcmd->file, 0, rcmd->efile - rcmd->file);
//         printf("REDIR: fd=%d file=[%s] mode=%d\n", rcmd->fd, filename, rcmd->mode);
//         free(filename);
//         debug_print_cmd(rcmd->cmd, depth + 1);
//     }
//     else if (cmd->type == PIPE) {
//         t_pipecmd *pcmd = (t_pipecmd *)cmd;
//         printf("PIPE:\n");
//         debug_print_cmd(pcmd->left, depth + 1);
//         debug_print_cmd(pcmd->right, depth + 1);
//     }
//     else if (cmd->type == LIST) {
//         t_listcmd *lcmd = (t_listcmd *)cmd;
//         printf("LIST:\n");
//         debug_print_cmd(lcmd->left, depth + 1);
//         debug_print_cmd(lcmd->right, depth + 1);
//     }
//     else if (cmd->type == BACK) {
//         t_backcmd *bcmd = (t_backcmd *)cmd;
//         printf("BACK:\n");
//         debug_print_cmd(bcmd->cmd, depth + 1);
//     }
//     else {
//         printf("UNKNOWN TYPE: %d\n", cmd->type);
//     }
// }

void	execution(char *buf, t_shell *shell)
{
	t_cmd	*cmd;

	cmd = parsecmd(buf);
	// printf("=== PARSED COMMAND ===\n");
	// debug_print_cmd(cmd, 0);
	// printf("===================\n");
	if (collect_all_heredocs(cmd, shell) < 0)
	{
		free_cmd(cmd);
		return ;
	}
	if (!should_fork(cmd))
	{
		if (cmd->type == EXEC)
			execute_builtin(cmd, shell);
	}
	else
	{
		execute_forked(cmd, shell);
	}
	free_cmd(cmd);
}
