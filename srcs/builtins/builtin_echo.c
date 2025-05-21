/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:35:01 by gansari           #+#    #+#             */
/*   Updated: 2025/05/13 17:51:03 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

static char	*remove_even_quotes(char *str)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	j = 0;
	temp = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!temp)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
			i++;
		else
			temp[j++] = str[i++];
	}
	temp[j] = '\0';
	return (temp);
}

static int	count_quotes(char *str)
{
	int		double_quote;
	int		single_quote;
	int		i;

	single_quote = 0;
	double_quote = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '"')
			double_quote++;
		else if (str[i] == '\'')
			single_quote++;
		i++;
	}
	if (!(single_quote == 0 && double_quote == 0))
	{
		if (single_quote % 2 == 0 && double_quote % 2 == 0)
			return (1);
	}
	return (0);
}

/**
 * is_valid_n_flag - Checks if a string represents a valid -n flag option
 * @str: The string to check
 *
 * This function validates whether a string is a valid -n option for echo.
 * Valid options include "-n", "-nn", "-nnn", etc. (one or more 'n' after '-')
 *
 * Return: 1 if valid -n flag, 0 otherwise
 */
static int	is_valid_n_flag(char *str)
{
	int	i;

	if (!str || str[0] != '-')
		return (0);
	if (str[1] == '\0')
		return (0);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

static char	*unquoted_str(char *str)
{
	if (count_quotes(str))
		return (remove_even_quotes(str));
	else
		return (remove_quotes(str));
}

/**
 * builtin_echo - Implements the echo builtin command
 * @ecmd: Command structure containing arguments to echo
 * @shell: Shell state structure (unused in this function)
 *
 * This function implements echo with support for the -n option.
 * It supports multiple -n flags (e.g., -nnnn) which all have the same effect
 * as a single -n flag. The -n flag suppresses the trailing newline.
 *
 * Examples:
 *   echo hello      -> hello\n
 *   echo -n hello   -> hello
 *   echo -nnnn hi   -> hi hi
 *   echo -n -nn hi  -> hi
 *
 * Return: Always 0 (success)
 */
int	builtin_echo(t_execcmd *ecmd, t_shell *shell)
{
	int		i;
	int		n_flag;
	char	*unquoted;

	(void)shell;
	n_flag = 0;
	i = 1;
	while (ecmd->argv[i] && is_valid_n_flag(ecmd->argv[i]))
	{
		n_flag = 1;
		i++;
	}
	while (ecmd->argv[i])
	{
		unquoted = unquoted_str(ecmd->argv[i]);
		ft_putstr_fd(unquoted, STDOUT_FILENO);
		free(unquoted);
		if (ecmd->argv[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (!n_flag)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}
