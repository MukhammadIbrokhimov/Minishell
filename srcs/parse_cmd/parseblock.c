/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseblock.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:00:46 by mukibrok          #+#    #+#             */
/*   Updated: 2025/04/22 15:12:05 by mukibrok         ###   ########.fr       */
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