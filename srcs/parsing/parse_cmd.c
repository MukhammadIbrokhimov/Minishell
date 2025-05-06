/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:17:52 by mukibrok          #+#    #+#             */
/*   Updated: 2025/05/06 14:29:05 by mukibrok         ###   ########.fr       */
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
	print_cmd(cmd);
	return (cmd);
}
