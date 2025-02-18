/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_env_token.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 07:47:50 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/18 17:56:32 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	replace_wrapper(char ****argv_ptr, char ***argv, int i,
		char ***split_tokens)
{
	**argv_ptr = replace_token_in_array(*argv, i, *split_tokens);
	if (!**argv_ptr)
	{
		free_str_array(*split_tokens);
		return (-1);
	}
	free_str_array(*split_tokens);
	return (0);
}

static int	process_expanded_token_with_spaces(char ***argv_ptr, char **argv,
		int i, char *expanded)
{
	char	**split_tokens;

	split_tokens = ft_splitter(expanded, ' ');
	free(expanded);
	if (!split_tokens)
		return (-1);
	if (!split_tokens || !split_tokens[0])
	{
		if (split_tokens)
			free_str_array(split_tokens);
		remove_argument(argv_ptr, i);
		return (0);
	}
	if (array_length(split_tokens) > 1)
		return (replace_wrapper(&argv_ptr, &argv, i, &split_tokens));
	else
	{
		free(argv[i]);
		argv[i] = ft_strdup(split_tokens[0]);
		free_str_array(split_tokens);
		if (!argv[i])
			return (-1);
	}
	return (1);
}

static int	finalize_expanded_token(int encl, char *expanded, char **argv,
		int i)
{
	char	*no_closed;

	if (encl != 2 && encl != 0 && encl != 1)
	{
		no_closed = strip_outers_quotes(expanded);
		free(expanded);
		if (!no_closed)
			return (0);
		free(argv[i]);
		argv[i] = no_closed;
	}
	else
	{
		free(argv[i]);
		argv[i] = expanded;
	}
	return (1);
}

int	process_env_token(char ***argv_ptr, t_shell *shell, int i)
{
	char	**argv;
	char	*expanded;
	int		encl;
	int		temp;

	argv = *argv_ptr;
	expanded = expand_and_cleanup_token(argv_ptr, argv, i, shell);
	if (!expanded)
		return (0);
	encl = count_surrounding_quotes(argv[i]);
	if (!encl && ft_strchr(expanded, ' '))
	{
		temp = process_expanded_token_with_spaces(argv_ptr, argv, i, expanded);
		if (temp <= 0)
			return (temp);
	}
	else
	{
		if (!finalize_expanded_token(encl, expanded, argv, i))
			return (-1);
	}
	return (1);
}
