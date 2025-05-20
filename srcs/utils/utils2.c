/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:29:28 by mukibrok          #+#    #+#             */
/*   Updated: 2025/05/20 16:29:39 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

int	if_only_token(const char *str)
{
	char	*ptr;

	if (!str)
		return (1);
	while (ft_isspace(*str))
		str++;
	ptr = ft_strchr(SYMBOLS, *str);
	if (ptr)
	{
		fprintf(stderr, "sadaf: syntax error near unexpected token '%c'\n",
			*ptr);
		return (1);
	}
	return (0);
}