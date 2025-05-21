/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:29:28 by mukibrok          #+#    #+#             */
/*   Updated: 2025/05/21 16:23:05 by mukibrok         ###   ########.fr       */
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
