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

static int	are_quotes_balanced(char *str)
{
	int	i;
	int	in_single_quote;
	int	in_double_quote;

	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		i++;
	}
	return (!in_single_quote && !in_double_quote);
}

static char	*get_continuation_input(char *initial_input)
{
	char	*line;
	char	*result;
	char	*temp;

	result = ft_strdup(initial_input);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break;
		temp = result;
		result = ft_strjoin(result, "\n");
		free(temp);
		temp = result;
		result = ft_strjoin(result, line);
		free(temp);
		free(line);
		if (are_quotes_balanced(result))
			break;
	}
	return (result);
}

static char	*combine_arguments(char **argv, int start_idx)
{
	int		i;
	char	*combined;
	char	*temp;

	i = start_idx;
	combined = ft_strdup("");
	while (argv[i])
	{
		if (i > start_idx)
		{
			temp = combined;
			combined = ft_strjoin(combined, " ");
			free(temp);
		}
		temp = combined;
		combined = ft_strjoin(combined, argv[i]);
		free(temp);
		i++;
	}
	return (combined);
}

/**
 * is_quote_char - Handles the quote character logic
 * 
 * This function determines if the current character is a quote character and
 * updates the quote state accordingly. It uses bit manipulation to track both
 * single and double quote states within a single integer:
 * - Bit 0 (LSB): single quote state (0 = not in single quote, 1 = in single quote)
 * - Bit 1: double quote state (0 = not in double quote, 1 = in double quote)
 *
 * @param c: The current character being processed
 * @param quote_state: Pointer to the integer tracking both quote states
 * 
 * @return: 1 if the character is a quote that should be processed, 0 otherwise
 */
static int	is_quote_char(char c, int *quote_state)
{
	int	in_single_quote;
	int	in_double_quote;

	in_single_quote = *quote_state & 1;
	in_double_quote = (*quote_state >> 1) & 1;
	if (c == '\'' && !in_double_quote)
	{
		*quote_state ^= 1;
		return (1);
	}
	else if (c == '"' && !in_single_quote)
	{
		*quote_state ^= 2;
		return (1);
	}
	return (0);
}

/**
 * process_quotes - Processes a string by removing quote characters
 * 
 * This function processes the input string by removing single and double quote
 * characters while respecting quoting rules. If a character is inside quotes of
 * the opposite type, it's preserved. For example, single quotes inside double 
 * quotes are preserved, and vice versa.
 *
 * The function uses only 4 variables as required:
 * - i: Index for iterating through the input string
 * - j: Index for writing to the result string
 * - quote_state: Integer to track both quote states using bit manipulation
 * - result: The output string with quotes removed
 *
 * @param input: The input string to process
 * 
 * @return: A newly allocated string with quotes removed, or NULL if memory allocation fails
 *          The caller is responsible for freeing this memory
 */
static char *process_quotes(char *input)
{
	int		i;
	int		j;
	int		quote_state;
	char	*result;

	i = 0;
	j = 0;
	quote_state = 0;
	result = malloc(sizeof(char) * (ft_strlen(input) + 1));
	if (!result)
		return NULL;
	while (input[i])
	{
		if (!is_quote_char(input[i], &quote_state))
			result[j++] = input[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

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

int	builtin_echo(t_execcmd *ecmd, t_shell *shell)
{
	int		n_flag;
	int		start_idx;
	char	*combined_input;
	char	*processed_output;

	(void)shell;
	n_flag = 0;
	start_idx = 1;
	if (!ecmd->argv[1])
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		return (0);
	}
	if (is_valid_n_flag(ecmd->argv[1]))
	{
		n_flag = 1;
		start_idx = 2;
		if (!ecmd->argv[2])
			return (0);
	}
	combined_input = combine_arguments(ecmd->argv, start_idx);
	if (!are_quotes_balanced(combined_input))
	{
		char *temp = combined_input;
		combined_input = get_continuation_input(combined_input);
		free(temp);
	}
	processed_output = process_quotes(combined_input);
	ft_putstr_fd(processed_output, STDOUT_FILENO);
	if (!n_flag)
		ft_putstr_fd("\n", STDOUT_FILENO);
	free(combined_input);
	free(processed_output);
	return (0);
}
