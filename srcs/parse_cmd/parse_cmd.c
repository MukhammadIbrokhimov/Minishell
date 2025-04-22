/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muxammad <muxammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:17:52 by mukibrok          #+#    #+#             */
/*   Updated: 2025/04/22 11:22:36 by muxammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"



/**
 * parseblock - Parses a command block enclosed in parentheses
 * @ps: Parser state containing input string and position
 * 
 * Returns: t_cmd* representing the parsed block command
 *          NULL on syntax error (caller should handle)
 *
 * Handles: ( command-list ) [redirections]
 * Performs full syntax validation and maintains parser state
 */

t_cmd *parseblock(ParserState *ps)
{
	t_cmd *cmd;
	t_token tok;
	
	/* --- Phase 1: Validate block opening --- */
	tok = gettoken(ps); // completed
	if (tok.type != TOK_LPAREN)
	{
		fprintf(stderr, "Syntax error: Expected '(' to start block at %.*s\n", 
				(int)(tok.end - tok.start), tok.start);
		return NULL;
	}

	/* --- Phase 2: Parse block contents --- */
	if (!(cmd = parseline(ps))) // completed
	{
		fprintf(stderr, "Error: Failed to parse block contents\n");
		return NULL;
	}

	/* --- Phase 3: Validate block closing --- */
	tok = gettoken(ps);
	if (tok.type != TOK_RPAREN)
	{
		fprintf(stderr, "Syntax error: Unclosed block, expected ')' at %.*s\n",
				(int)(ps->end - tok.start), tok.start);
		//freecmd(cmd);  // Avoid memory leak
		return (NULL);
	}

	/* --- Phase 4: Handle trailing redirections --- */
	if (!(cmd = parseredirs(cmd, ps)))
	{
		fprintf(stderr, "Error: Failed to parse redirections for block\n");
		//freecmd(cmd);
		return (NULL);
	}

	return (cmd);
}

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
		cmd = pipecmd(cmd, parsepipe(ps)); // completed
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
			cmd = backcmd(cmd); // completed
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
	ps.end = buf + strlen(buf);
	cmd = parseline(&ps); // developing
	tok = gettoken(&ps); // completed
	if (tok.type != TOK_EOF)
	{
		fprintf(stderr, "syntax error: unexpected token at '%.*s'\n",
			(int)(tok.end - tok.start), tok.start);
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
    if(buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' '){
      // Chdir must be called by the parent, not the child.
      buf[strlen(buf)-1] = 0;  // chop \n
      if(chdir(buf+3) < 0)
        fprintf(stderr, "cannot cd %s\n", buf+3);
      continue;
    }
    if(fork1() == 0){
		t_cmd *cmd = parsecmd(buf);
		printf("command: %d\n", cmd->type);
	}
    wait(NULL);
  }
  
  exit(0);
}