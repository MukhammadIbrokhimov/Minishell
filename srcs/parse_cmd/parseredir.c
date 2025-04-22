/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseredir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muxammad <muxammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:57:14 by muxammad          #+#    #+#             */
/*   Updated: 2025/04/22 11:12:17 by muxammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * parseredirs - Parses redirection operators and their target files
 * @cmd: Command to attach redirections to
 * @ps: Parser state containing input string and position
 *
 * Handles: < file, > file, >> file
 * Returns: Modified command with redirections attached
 *          Original command on syntax error (caller should check)
 */

t_cmd *parseredirs(t_cmd *cmd, ParserState *ps)
{
	t_token op_tok, file_tok;
	
	while (1) {
		// Peek next token to check for redirection operators
		op_tok = gettoken(ps);
		if (op_tok.type != TOK_LT && op_tok.type != TOK_GT && op_tok.type != TOK_DGT) {
			ps->s = op_tok.start; // Put token back
			break;
		}

		// Get the redirection target file
		file_tok = gettoken(ps);
		if (file_tok.type != TOK_WORD) {
			fprintf(stderr, "Syntax error: Expected filename after redirection at %.*s\n",
				(int)(file_tok.end - file_tok.start), file_tok.start);
			return cmd;
		}

		// Create redirection command node
		int mode, fd;
		switch (op_tok.type) {
			case TOK_LT:    // <
				mode = O_RDONLY;
				fd = 0;     // stdin
				break;
			case TOK_GT:    // >
				mode = O_WRONLY | O_CREAT | O_TRUNC;
				fd = 1;     // stdout
				break;
			case TOK_DGT:   // >>
				mode = O_WRONLY | O_CREAT | O_APPEND;
				fd = 1;     // stdout
				break;
			default:
				fprintf(stderr, "Internal error: Unknown redirection operator\n");
				return cmd;
		}

		t_cmd *newcmd = redircmd(cmd, file_tok.start, file_tok.end, mode, fd);
		if (!newcmd) {
			fprintf(stderr, "Error: Failed to create redirection command\n");
			return cmd;
		}
		cmd = newcmd;
	}

	return cmd;
}
