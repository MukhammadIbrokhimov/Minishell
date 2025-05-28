/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:11:20 by gansari           #+#    #+#             */
/*   Updated: 2025/05/28 17:23:00 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

char	*append_char(char *expanded, char c)
{
	char	tmp_str[2];
	char	*tmp;

	tmp_str[0] = c;
	tmp_str[1] = '\0';
	tmp = expanded;
	expanded = ft_strjoin(expanded, tmp_str);
	free(tmp);
	return (expanded);
}

char	*process_arg(char *arg, t_shell *shell)
{
	int		j;
	char	*expanded;

	expanded = ft_strdup("");
	j = 0;
	while (arg[j])
	{
		if (arg[j] == '$' && !is_inside_single_quotes(arg, j))
		{
			if (arg[j + 1] == '?')
			{
				expanded = handle_exit_status(expanded, shell);
				j += 2;
				continue ;
			}
			expanded = expand_var(expanded, arg, &j, shell);
		}
		else
			expanded = append_char(expanded, arg[j]);
		j++;
	}
	return (expanded);
}

static void	handle_empty_argv(t_execcmd *ecmd, int i)
{
	int	j;

	j = i;
	while (ecmd->argv[j + 1])
	{
		ecmd->argv[j] = ecmd->argv[j + 1];
		j++;
	}
	ecmd->argv[j] = NULL;
}

static int	is_empty_after_expansion(char *expanded)
{
	int	i;

	if (!expanded)
		return (1);
	i = 0;
	while (expanded[i])
	{
		if (!ft_isspace(expanded[i]))
			return (0);
		i++;
	}
	return (1);
}

void	expand_variables(t_execcmd *ecmd, t_shell *shell)
{
	int		i;
	char	*dollar;
	char	*expanded;
	char	*tilde_expanded;

	i = 0;
	while (ecmd->argv[i])
	{
		tilde_expanded = expand_tilde(ecmd->argv[i], shell);
		dollar = ft_strchr(tilde_expanded, '$');
		if (dollar)
		{
			expanded = process_arg(tilde_expanded, shell);
			free(tilde_expanded);
			if (is_empty_after_expansion(expanded))
			{
				free(expanded);
				handle_empty_argv(ecmd, i);
				continue ;
			}
			else
				ecmd->argv[i] = expanded;
		}
		else
			ecmd->argv[i] = tilde_expanded;
		i++;
	}
}

