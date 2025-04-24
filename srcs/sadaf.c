/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sadaf.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:38:55 by mukibrok          #+#    #+#             */
/*   Updated: 2025/04/24 18:05:48 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sadaf.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	char	*buf;

	(void)argc;
	(void)argv;
	shell = init_shell(envp);
	if (!shell)
		return (perror("Failed to initialize shell"), EXIT_FAILURE);
	while(1)
	{
		buf = getcmd();
		if(buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' ')
		{
			buf[strlen(buf)-1] = 0;
			if(chdir(buf+3) < 0)
				fprintf(stderr, "cannot cd %s\n", buf+3);
			continue;
		}
		if(fork1() == 0)
		{
			t_cmd *cmd = parsecmd(buf);
			runcmd(cmd, shell);
			free_cmd(cmd);
			exit(EXIT_SUCCESS);
		}
		wait(NULL);
		free(buf);
	}
	free_shell(shell);
	return (EXIT_SUCCESS);
}

//int
//main(int argc, char **argv, char **envp)
//{
//	int fd;
//	char *buf;
//	t_shell *shell;
//	(void)argc;
//	(void)argv;
//	shell = init_shell(envp);
//	if (!shell)
//		return (perror("Failed to initialize shell"), EXIT_FAILURE);
//	while(1){
//	buf = getcmd();
//	if (!buf)
//		break ;
//	if (buf[0] == '~')
//	{
//		free(buf);
//		break ;
//	}
//	if(buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' '){
//		// Chdir must be called by the parent, not the child.
//		buf[strlen(buf)-1] = 0;  // chop \n
//		if(chdir(buf+3) < 0)
//		fprintf(stderr, "cannot cd %s\n", buf+3);
//		free(buf);
//		continue;
//	}
//	if(fork1() == 0){
//		t_cmd *cmd = parsecmd(buf);
//		runcmd(cmd, shell);
//		//if (cmd->type == EXEC) {
//		//	t_execcmd *ecmd = (t_execcmd *)cmd;
//		//	printf("EXEC command: argv[0] = %s\n", ecmd->argv[0]);
//		//} else if (cmd->type == REDIR) {
//		//	t_redircmd *rcmd = (t_redircmd *)cmd;
//		//	printf("REDIR command: file = %s\n", rcmd->file);
//		//} else if (cmd->type == PIPE) {
//		//	t_pipecmd *pcmd = (t_pipecmd *)cmd;
//		//	printf("PIPE command: left type = %d, right type = %d\n", pcmd->left->type, pcmd->right->type);
//		//} else if (cmd->type == LIST) {
//		//	t_listcmd *lcmd = (t_listcmd *)cmd;
//		//	printf("LIST command: left type = %d, right type = %d\n", lcmd->left->type, lcmd->right->type);
//		//} else if (cmd->type == BACK) {
//		//	t_backcmd *bcmd = (t_backcmd *)cmd;
//		//	printf("BACK command: cmd type = %d\n", bcmd->cmd->type);
//		//} else {
//		//	fprintf(stderr, "Unknown command type\n");
//		//}
//		free_cmd(cmd);
//		free_shell(shell);
//		exit(0);
//	}
//		wait(NULL);
//		free(buf);
//	}
//	clear_history();
//	exit(0);
//}