/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parce_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muxammad <muxammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:48:08 by muxammad          #+#    #+#             */
/*   Updated: 2025/04/21 15:28:29 by muxammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"
#define SYMBOLS "|&;()<>"

t_token get_token(ParserState *ps)
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
	switch(*s)
	{
		case '|': tok.type = TOK_PIPE; s++; break;
		case '&': tok.type = TOK_AND; s++; break;
		case ';': tok.type = TOK_SEQ; s++; break;
		case '<': tok.type = TOK_LT; s++; break;
		case '>':
		s++;
		if(*s == '>') { tok.type = TOK_DGT; s++; }
		else tok.type = TOK_GT;
		break;
		case '(': tok.type = TOK_LPAREN; s++; break;
		case ')': tok.type = TOK_RPAREN; s++; break;
		default:
		tok.type = TOK_WORD;
		while(s < ps->end && !isspace(*s) && !strchr(SYMBOLS, *s))
			s++;
		break;
	}
	tok.end = s;
	ps->s = s;
	return (tok);
}