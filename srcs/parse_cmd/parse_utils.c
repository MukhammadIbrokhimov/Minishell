/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:48:08 by muxammad          #+#    #+#             */
/*   Updated: 2025/04/24 15:13:35 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"
#define SYMBOLS "|&;()<>"

 /**
 * gettoken - Breaks down shell commands into meaningful parts
 * @ps: Tracks current position in the command string
 * 
 * What it does:
 * - Finds special characters like pipes (|), arrows (< >), semicolons (;)
 * - Spots command pieces (like program names and arguments)
 * - Notices when we reach the end of the command
 * - Handles parentheses for command grouping
 * 
 * Returns a package containing:
 *   - Type: What kind of part it found (pipe, word, etc.)
 *   - Location: Where this part starts and ends in the command
 */

void	assign_token(t_token *tok, char **s)
{
	switch(**s)
	{
		case '|': (*tok).type = TOK_PIPE; (*s)++; break;
		case '&': (*tok).type = TOK_AND; (*s)++; break;
		case ';': (*tok).type = TOK_SEQ; (*s)++; break;
		case '<': (*tok).type = TOK_LT; (*s)++; break;
		case '>':
			(*s)++;
			if(**s == '>') { (*tok).type = TOK_DGT; (*s)++; }
			else (*tok).type = TOK_GT;
			break;
		case '(': (*tok).type = TOK_LPAREN; (*s)++; break;
		case ')': (*tok).type = TOK_RPAREN; (*s)++; break;
		default:
			(*tok).type = TOK_WORD;
		break;
	}
}

t_token gettoken(ParserState *ps)
{
	char *s;
	t_token tok;
	
	s = ps->s;
	while(s < ps->end && isspace(*s))
		s++;
	tok.start = s;
	if(s >= ps->end){
		tok.type = TOK_EOF;
		tok.end = s;
		ps->s = s;
		return tok;
	}
	assign_token(&tok, &s);
	if(tok.type == TOK_WORD)
	{
		while(s < ps->end && !isspace(*s) && !strchr(SYMBOLS, *s))
			s++;
	}
	tok.end = s;
	ps->s = s;
	return (tok);
}