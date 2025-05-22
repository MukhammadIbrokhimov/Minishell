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
