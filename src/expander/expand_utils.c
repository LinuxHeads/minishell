/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 07:22:27 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/16 07:38:24 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_tokens(char **arr, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

int	check_for_quotes(char *old_arg, char **arg)
{
	char	*c;

	if (is_single_quote_token(old_arg) || is_double_quote_token(old_arg))
	{
		c = malloc(3 * sizeof(char));
		if (!c)
			return (2);
		c[0] = old_arg[0];
		c[1] = old_arg[1];
		c[2] = '\0';
		free(old_arg);
		*arg = c;
		return (1);
	}
	return (0);
}

int	has_closing(int i, char quote, const char *s, size_t len)
{
	size_t	k;

	k = i + 1;
	while (k < len)
	{
		if (s[k] == quote)
		{
			return (1);
			break ;
		}
		k++;
	}
	return (0);
}

void	remove_argument(char ***argv_ptr, int index)
{
	char	**argv;
	int		i;

	argv = *argv_ptr;
	i = index;
	free(argv[i]);
	while (argv[i + 1])
	{
		argv[i] = argv[i + 1];
		i++;
	}
	argv[i] = NULL;
}

int	check_for_sign(char *old_arg, char **arg)
{
	char	*no_closed_quotes;
	char	*tmp;

	if (!ft_strchr(old_arg, '$'))
	{
		no_closed_quotes = strip_outers_quotes(old_arg);
		if (no_closed_quotes)
		{
			free(old_arg);
			old_arg = no_closed_quotes;
			*arg = old_arg;
		}
		tmp = cleanup_input(old_arg);
		if (tmp)
		{
			free(old_arg);
			*arg = tmp;
		}
		return (1);
	}
	return (0);
}
