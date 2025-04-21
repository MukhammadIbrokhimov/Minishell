/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muxammad <muxammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:17:52 by mukibrok          #+#    #+#             */
/*   Updated: 2025/04/21 15:59:31 by muxammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

t_cmd	*parseexec(ParserState *ps)
{
	t_execcmd	*cmd;
	t_cmd		*ret;
	t_token		tok;
	int			argc;

	tok = get_token(ps);
	if (tok.type == TOK_LPAREN)
		return (parseblock(ps)); // not completed
	ps->s = tok.start;
	ret = execcmd(); // not completed
	cmd = (t_execcmd *)ret;
	argc = 0;
	ret = parseredirs(ret, ps); // not completed
	while (1)
	{
		tok = get_token(ps);
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
		ret = parseredirs(ret, ps); // not completed
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
	tok = get_token(ps); // completed
	if (tok.type == TOK_PIPE) // if |
		cmd = pipecmd(cmd, parsepipe(ps)); // not completed
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
		tok = get_token(ps); // tokenize completed
		if (tok.type == TOK_AND) // if &
			cmd = backcmd(cmd); // not completed
		else if (tok.type == TOK_SEQ) // if ;
			cmd = listcmd(cmd, parseline(ps)); // not completed
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
	cmd = parse_line(&ps); // developing
	tok = get_token(&ps); // completed
	if (tok.type != TOK_EOF)
	{
		fprintf(stderr, "syntax error: unexpected token at '%.*s'\n",
			(int)(tok.end - tok.start), tok.start);
		exit(EXIT_FAILURE);
	}
	return (cmd);
}
