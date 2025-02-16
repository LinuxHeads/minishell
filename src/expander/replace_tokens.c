/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 07:45:11 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/16 07:50:29 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	copy_tokens(char **dest, char **src, int *st_en_pos)
{
	while (st_en_pos[0] < st_en_pos[1])
	{
		dest[st_en_pos[2]] = ft_strdup(src[st_en_pos[0]]);
		if (!dest[st_en_pos[2]])
		{
			return (-1);
		}
		st_en_pos[2]++;
		st_en_pos[0]++;
	}
	return (st_en_pos[2]);
}

static void	free_old_argv(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

static int	assemble_replacement_argv(int index, char **new_argv, char **argv,
		char **new_tokens)
{
	int	pos;
	int	temp;
	int	count;

	count = array_length(new_tokens);
	pos = copy_tokens(new_argv, argv, (int []){0, index, 0});
	if (pos == -1)
		return (-2);
	temp = pos;
	pos = copy_tokens(new_argv, new_tokens, (int []){0, count, pos});
	if (pos == -1)
	{
		free_tokens(new_argv, temp);
		return (0);
	}
	count = array_length(argv);
	pos = copy_tokens(new_argv, argv, (int []){index + 1, count, pos});
	if (pos == -1)
	{
		free_tokens(new_argv, temp);
		return (0);
	}
	return (pos);
}

char	**replace_token_in_array(char **argv, int index, char **new_tokens)
{
	int		old_count;
	int		new_count;
	char	**new_argv;
	int		pos;

	old_count = array_length(argv);
	new_count = array_length(new_tokens);
	new_argv = malloc(sizeof(char *) * (old_count - 1 + new_count + 1));
	if (!new_argv)
		return (NULL);
	pos = assemble_replacement_argv(index, new_argv, argv, new_tokens);
	if (!pos || pos == -2)
	{
		if (pos == -2)
			free(new_argv);
		return (NULL);
	}
	free_old_argv(argv);
	new_argv[pos] = NULL;
	return (new_argv);
}
