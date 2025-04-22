/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:17:52 by mukibrok          #+#    #+#             */
/*   Updated: 2025/04/22 15:09:10 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

t_cmd	*parseexec(ParserState *ps)
{
	t_execcmd	*cmd;
	t_cmd		*ret;
	t_token		tok;
	int			argc;

	tok = gettoken(ps); // completed
	if (tok.type == TOK_LPAREN)
		return (parseblock(ps)); // completed
	ps->s = tok.start;
	ret = execcmd(); // completed
	cmd = (t_execcmd *)ret;
	argc = 0;
	ret = parseredirs(ret, ps); // completed
	while (1)
	{
		tok = gettoken(ps); // completed
		if (tok.type == TOK_PIPE || tok.type == TOK_AND
			|| tok.type == TOK_SEQ || tok.type == TOK_RPAREN
			|| tok.type == TOK_EOF)
		{
			ps->s = tok.start;
			break ;
		}
		if (tok.type != TOK_WORD)
			fprintf(stderr, "syntax error: expected argument");
		if (argc >= MAXARGS)
			fprintf(stderr, "too many args");
		cmd->argv[argc] = tok.start;
		cmd->eargv[argc] = tok.end;
		argc++;
		ret = parseredirs(ret, ps); // completed
	}
	cmd->argv[argc] = 0;
	cmd->eargv[argc] = 0;
	return (ret);
}

t_cmd	*parsepipe(ParserState *ps)
{
	t_cmd	*cmd;
	t_token	tok;

	cmd = parseexec(ps); // developing
	tok = gettoken(ps); // completed
	if (tok.type == TOK_PIPE) // if |
	{
		cmd = pipecmd(cmd, parsepipe(ps)); // completed
		if (!cmd)
			fprintf(stderr, "Pipe command failed\n");
	}
	else
		ps->s = tok.start;
	return (cmd);
}

t_cmd	*parseline(ParserState *ps)
{
	t_cmd	*cmd;
	t_token	tok;

	cmd = parsepipe(ps); // developing
	while (1)
	{
		tok = gettoken(ps); // tokenize completed
		if (tok.type == TOK_AND) // if &
		{
			cmd = backcmd(cmd); // completed
			if (!cmd)
				fprintf(stderr, "Backcmd failed\n");
		}
		else if (tok.type == TOK_SEQ) // if ;
			cmd = listcmd(cmd, parseline(ps)); // completed
		else
		{
			ps->s = tok.start;
			break ;
		}
	}
	return (cmd);
}

t_cmd	*parsecmd(char *buf)
{
	ParserState	ps;
	t_cmd		*cmd;
	t_token		tok;

	ps.s = buf;
	ps.end = buf + ft_strlen(buf);
	cmd = parseline(&ps); // completed
	tok = gettoken(&ps); // completed
	if (tok.type != TOK_EOF)
	{
		fprintf(stderr, "syntax error: unexpected token at '%.*s'\n",
			(int)(tok.end - tok.start), tok.start);
		free_cmd(cmd); // free the command structure
		exit(EXIT_FAILURE);
	}
	return (cmd);
}

int fork1()
{
	int	pid;

	pid = fork();
	if (pid < 0)
		fprintf(stderr, "Process error\n");
	return (pid);
}

int
main(void)
{
  int fd;
  char *buf;

  // Ensure that three file descriptors are open.
  while((fd = open("console", O_RDWR)) >= 0){
    if(fd >= 3){ // if we have more than stdin stdout and stderr close file descriptor
      close(fd);
      break;
    }
  }

  // Read and run input commands.
  while(1){
	buf = getcmd();
	if (!buf)
		break ;
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
  
  exit(0);
}