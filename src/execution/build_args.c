/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 06:18:02 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/16 12:39:59 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_redirection_token(int type)
{
	return (type == REDIRECT_IN || type == REDIRECT_OUT
		|| type == REDIRECT_APPEND || type == HEREDOC);
}

static int	count_command_arguments(t_command *cmd)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (i < cmd->token_count)
	{
		if (is_redirection_token(cmd->tokens[i]->type))
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
		if (is_redirection_token(cmd->tokens[i]->type))
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

	count = count_command_arguments(cmd);
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
/*
//test for leaks
int	main(void)
{
	t_command	*cmd;
	char		**argv;
	int			i;

	cmd = malloc(sizeof(t_command));
	cmd->token_count = 5;
	cmd->tokens = malloc(sizeof(t_token *) * 6);
	i = 0;
	while (i < 5)
	{
		cmd->tokens[i] = malloc(sizeof(t_token));
		cmd->tokens[i]->type = COMMAND;
		cmd->tokens[i]->value = ft_strdup("ls");
		i++;
	}
	cmd->tokens[i] = NULL;
	argv = build_command_argv(cmd);
	i = 0;
	while (argv[i])
	{
		printf("%s\n", argv[i]);
		i++;
	}
	free_str_array(argv);
	free(cmd->tokens[0]->value);
	free(cmd->tokens[1]->value);
	free(cmd->tokens[2]->value);
	free(cmd->tokens[3]->value);
	free(cmd->tokens[4]->value);
	free_str_array(cmd->tokens);
	free(cmd);
	return (0);
}
*/
