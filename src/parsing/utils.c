/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 07:13:16 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/06 07:25:43 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	count_words(char *str, char sep)
{
	int		count;
	char	**tokens;

	count = 0;
	tokens = ft_splitter(str, sep);
	if (!tokens)
		return (0);
	while (tokens[count])
		count++;
	free_str_array(tokens);
	return (count);
}

void	free_shell(t_exec *shell)
{
	int	i;
	int	j;

	if (!shell)
		return ;
	i = 0;
	while (i < shell->command_count)
	{
		j = 0;
		while (j < shell->commands[i]->token_count)
		{
			free(shell->commands[i]->tokens[j]->value);
			free(shell->commands[i]->tokens[j]);
			j++;
		}
		free(shell->commands[i]->tokens);
		free(shell->commands[i]);
		i++;
	}
	free(shell->commands);
	free(shell);
}

const char	*get_token_type_name(t_token_type type)
{
	const char	*names[] = {"COMMAND", "ARGUMENT", "PIPE", "REDIRECT_IN",
			"REDIRECT_OUT", "REDIRECT_APPEND", "HEREDOC", "HEREDOC_DELI",
			"ENV_VAR", "DOLLAR_SIGN", "SINGLE_QUOTE", "DOUBLE_QUOTE",
			"INPUT_FILE", "OUTPUT_FILE"};

	return (names[type]);
}

void	print_shell(t_exec *shell)
{
	int i;
	int j;

	printf("Parsed Shell Commands:\n");
	i = 0;
	while (i < shell->command_count)
	{
		printf("Command %d:\n", i + 1);
		j = 0;
		while (j < shell->commands[i]->token_count)
		{
			printf("  Token %d: %s (Type: %s)\n", j + 1,
				shell->commands[i]->tokens[j]->value,
				get_token_type_name(shell->commands[i]->tokens[j]->type));
			j++;
		}
		i++;
	}
}
