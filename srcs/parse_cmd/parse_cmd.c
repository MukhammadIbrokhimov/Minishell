/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:17:52 by mukibrok          #+#    #+#             */
/*   Updated: 2025/04/22 18:40:27 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * parsepipe - Processes piped command sequences
 * @ps: Tracks parsing position in input string
 * 
 * What it does:
 * - Starts with basic commands (parseexec)
 * - If finds pipe symbol "|", chains commands
 * - Handles multiple pipes (recursive approach)
 * - Example: "ls | grep txt | wc -l"
 * 
 * Throws errors if:
 * - Pipe symbol appears without commands
 * - Memory allocation fails
 * 
 * Returns: Command structure with pipe connections
 */

t_cmd	*parsepipe(ParserState *ps)
{
	t_cmd	*cmd;
	t_token	tok;

	cmd = parseexec(ps);
	tok = gettoken(ps);
	if (tok.type == TOK_PIPE)
	{
		cmd = pipecmd(cmd, parsepipe(ps));
		if (!cmd)
			ft_exit("Pipecmd failed\n");
	}
	else
		ps->s = tok.start;
	return (cmd);
}

/**
 * parseline - Manages command separators (; and &)
 * @ps: Tracks current parsing position
 * 
 * Handles:
 * - Command sequencing with ";"
 * - Background execution with "&"
 * - Recursive processing of multiple separators
 * - Example: "ls &; ps ; top"
 * 
 * Throws errors if:
 * - Invalid separator placement
 * - Memory allocation fails
 * 
 * Returns: Command structure with sequencing/background info
 */

t_cmd	*parseline(ParserState *ps)
{
	t_cmd	*cmd;
	t_token	tok;

	cmd = parsepipe(ps);
	while (1)
	{
		tok = gettoken(ps);
		if (tok.type == TOK_AND)
		{
			cmd = backcmd(cmd);
			if (!cmd)
				ft_exit("Backcmd failed\n");
		}
		else if (tok.type == TOK_SEQ)
		{
			cmd = listcmd(cmd, parseline(ps));
			if (!cmd)
				ft_exit("Listcmd failed\n");
		}
		else
		{
			ps->s = tok.start;
			break ;
		}
	}
	return (cmd);
}

/**
 * parsecmd - Converts raw command text into executable structure
 * @buf: User's input command string
 * 
 * What it does:
 * - Prepares command for processing
 * - Converts text to command structure
 * - Checks for leftover unprocessed text
 * - Handles final syntax validation
 * 
 * Throws errors if:
 * - Extra text remains after command
 * - Invalid command structure
 * - Memory allocation fails
 * 
 * Returns: Ready-to-execute command structure
 */

t_cmd	*parsecmd(char *buf)
{
	ParserState	ps;
	t_cmd		*cmd;
	t_token		tok;

	ps.s = buf;
	ps.end = buf + ft_strlen(buf);
	cmd = parseline(&ps);
	tok = gettoken(&ps);
	if (tok.type != TOK_EOF)
	{
		free_cmd(cmd);
		ft_exit("syntax error: unexpected token\n");
	}
	return (cmd);
}

int
main(void)
{
	int fd;
	char *buf;

	while((fd = open("console", O_RDWR)) >= 0){
		if(fd >= 3){ // if we have more than stdin stdout and stderr close file descriptor
		close(fd);
		break;
		}
	}
	while(1){
	buf = getcmd();
	if (!buf)
		break ;
	if (buf[0] == '~')
	{
		free(buf);
		break ;
	}
	if(buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' '){
		// Chdir must be called by the parent, not the child.
		buf[strlen(buf)-1] = 0;  // chop \n
		if(chdir(buf+3) < 0)
		fprintf(stderr, "cannot cd %s\n", buf+3);
		free(buf);
		continue;
	}
	if(fork1() == 0){
		t_cmd *cmd = parsecmd(buf);
		if (cmd->type == EXEC) {
			t_execcmd *ecmd = (t_execcmd *)cmd;
			printf("EXEC command: argv[0] = %s\n", ecmd->argv[0]);
		} else if (cmd->type == REDIR) {
			t_redircmd *rcmd = (t_redircmd *)cmd;
			printf("REDIR command: file = %s\n", rcmd->file);
		} else if (cmd->type == PIPE) {
			t_pipecmd *pcmd = (t_pipecmd *)cmd;
			printf("PIPE command: left type = %d, right type = %d\n", pcmd->left->type, pcmd->right->type);
		} else if (cmd->type == LIST) {
			t_listcmd *lcmd = (t_listcmd *)cmd;
			printf("LIST command: left type = %d, right type = %d\n", lcmd->left->type, lcmd->right->type);
		} else if (cmd->type == BACK) {
			t_backcmd *bcmd = (t_backcmd *)cmd;
			printf("BACK command: cmd type = %d\n", bcmd->cmd->type);
		} else {
			fprintf(stderr, "Unknown command type\n");
		}
		free_cmd(cmd);
		//free(buf);
		exit(0);
	}
	free(buf);
	wait(NULL);
	}
	clear_history();
	exit(0);
}