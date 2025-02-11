/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 06:18:02 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/11 21:58:22 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	is_redirect_operator(int type)
{
	return (type == REDIRECT_IN || type == REDIRECT_OUT
		|| type == REDIRECT_APPEND || type == HEREDOC);
}

static int	get_count(t_command *cmd)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (i < cmd->token_count)
	{
		if (is_redirect_operator(cmd->tokens[i]->type))
			i += 2;
		else
		{
			count++;
			i++;
		}
	}
	return (count);
}

static int	fill_args(char **argv, t_command *cmd)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < cmd->token_count)
	{
		if (is_redirect_operator(cmd->tokens[i]->type))
		{
			i += 2;
			continue ;
		}
		argv[j] = ft_strdup(cmd->tokens[i]->value);
		if (!argv[j])
		{
			free_str_array(argv);
			return (0);
		}
		j++;
		i++;
	}
	argv[j] = NULL;
	return (1);
}

char	**build_command_argv(t_command *cmd)
{
	int		count;
	char	**argv;

	count = get_count(cmd);
	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	if (!fill_args(argv, cmd))
	{
		free(argv);
		return (NULL);
	}
	return (argv);
}
