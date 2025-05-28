/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 17:14:49 by gansari           #+#    #+#             */
/*   Updated: 2025/05/28 18:08:39 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * expand_tilde - Expands ~ to home directory
 * @path: The path that might start with ~
 * @shell: Shell state containing environment variables
 * @return: Expanded path or NULL if HOME not set
 */
char	*expand_tilde(char *arg, t_shell *shell)
{
	char	*home;
	char	*result;

	if (!arg || arg[0] != '~')
		return (ft_strdup(arg));
	home = ft_getenv("HOME", shell);
	if (!home)
	{
		home = getenv("HOME");
		if (!home)
		{
			ft_putstr_fd("\x1b[31msadaf: cd: HOME not set\n", STDERR_FILENO);
			return (NULL);
		}
	}
	if (arg[1] == '\0')
		return (ft_strdup(home));
	else if (arg[1] == '/')
	{
		result = ft_strjoin(home, arg + 1);
		return (result);
	}
	else
		return (ft_strdup(arg));
}
