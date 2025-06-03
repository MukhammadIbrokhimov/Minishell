/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:29:28 by mukibrok          #+#    #+#             */
/*   Updated: 2025/06/03 18:28:27 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

int	if_contains_lparen(const char *str)
{
	if (!str)
		return (1);
	while (ft_isspace(*str))
		str++;
	while (*str)
	{
		if (*str == 34 || *str == 39)
			return (0);
		if (*str == '(')
		{
			ft_fprintf(2,
				"\x1b[31msadaf: syntax error near unexpected token '('\n");
			return (2);
		}
		if (*str == ')')
		{
			ft_fprintf(2,
				"\x1b[31msadaf: syntax error near unexpected token ')'\n");
			return (2);
		}
		str++;
	}
	return (0);
}

int	if_contains_doubleAndorPipe(const char *str)
{
	if (!str)
		return (1);
	while (ft_isspace(*str))
		str++;
	while (*str)
	{
		if (*str == '&' && *(str + 1) == '&')
		{
			ft_fprintf(2,
				"\x1b[31msadaf: syntax error near unexpected token '&&'\n");
			return (2);
		}
		if (*str == '|' && *(str + 1) == '|')
		{
			ft_fprintf(2,
				"\x1b[31msadaf: syntax error near unexpected token '||'\n");
			return (2);
		}
		str++;
	}
	return (0);
}