/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 02:02:23 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/11 22:58:34 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_pipes(t_exec *parser)
{
	int	i;

	if (!parser || !parser->commands)
		return (0);
	i = 0;
	while (i < parser->command_count)
	{
		/* Check if the command itself or its tokens array is NULL,
			or if the tokens array is empty (first token is NULL) */
		if (!parser->commands[i] || !parser->commands[i]->tokens
			|| !parser->commands[i]->tokens[0])
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `|'\n",
				2);
			return (0);
		}
		i++;
	}
	return (1);
}

/* Helper: returns 1 if the token is a redirection operator, 0 otherwise */
int	is_redirection_operator(const char *value)
{
	return (!ft_strcmp(value, ">") || !ft_strcmp(value, ">>")
		|| !ft_strcmp(value, "<") || !ft_strcmp(value, "<<"));
}

/* Extended check_redirections:
   - Loops through each command (if you have multiple commands in the pipeline)
   - For each redirection operator,
	ensures the following token exists and is not an operator */
int	check_redirections(t_exec *parser)
{
	int			i;
	int			cmd_index;
	t_command	*cmd;

	cmd_index = 0;
	while (parser->commands[cmd_index])
	{
		cmd = parser->commands[cmd_index];
		i = 0;
		while (cmd->tokens[i])
		{
			if (is_redirection_operator(cmd->tokens[i]->value))
			{
				if (!cmd->tokens[i + 1] || is_redirection_operator(cmd->tokens[i
						+ 1]->value) || !ft_strcmp(cmd->tokens[i + 1]->value,
						"|"))
				{
					ft_putstr_fd("minishell: syntax error near unexpected token `",
						2);
					if (cmd->tokens[i + 1])
						ft_putstr_fd(cmd->tokens[i + 1]->value, 2);
					else
						ft_putstr_fd("newline", 2);
					ft_putstr_fd("'\n", 2);
					return (0);
				}
			}
			i++;
		}
		cmd_index++;
	}
	return (1);
}

int	syntax_checker(t_exec *parser)
{
	if (!check_pipes(parser))
		return (0);
	if (!check_redirections(parser))
		return (0);
	return (1);
}
