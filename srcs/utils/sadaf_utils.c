/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sadaf_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:38:55 by mukibrok          #+#    #+#             */
/*   Updated: 2025/05/26 11:32:29 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

static int	is_only_whitespace(const char *str)
{
	if (!str)
		return (1);
	while (*str)
	{
		if (!ft_isspace(*str))
			return (0);
		str++;
	}
	return (1);
}

char *expand_env_vars(char *str)
{
	if (!str || *str != '$')
		return (str);

	if (str[0] == '$')
	{
		char *env_name = str + 1;
		char *env_value = getenv(env_name);
		return (env_value ? env_value : str);
	}
	return (str);
}

void	prepare_for_command(t_shell *shell)
{
	setup_signals(0, shell);
	g_signal_received = 0;
}

int	should_skip_empty_command(char *buf)
{
	if (is_only_whitespace(buf))
		return (1);
	if (buf[0] == '\n' || buf[0] == '\0')
		return (1);
	return (0);
}
