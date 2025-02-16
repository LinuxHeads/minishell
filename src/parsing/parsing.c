/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 02:35:03 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/16 13:07:59 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_token	*allocate_token(char *str, t_next_token *decide)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(str);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->type = identify_token_type(str, decide);
	return (token);
}

static int	fill_command_tokens(t_command *cmd, char **t_str,
		t_next_token *decide)
{
	int	j;

	j = 0;
	while (t_str[j])
	{
		cmd->tokens[j] = allocate_token(t_str[j], decide);
		if (!cmd->tokens[j])
			return (-1);
		j++;
	}
	cmd->tokens[j] = NULL;
	return (0);
}

static t_command	*allocate_command(char *cmd_str, int *i)
{
	t_command		*cmd;
	t_next_token	decide;
	char			**t_str;

	(*i)++;
	decide = (t_next_token){1, 0, 0, 0, 0, 0};
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->token_count = count_words(cmd_str, ' ');
	cmd->tokens = malloc(sizeof(t_token *) * (cmd->token_count + 1));
	if (!cmd->tokens)
	{
		free(cmd);
		return (NULL);
	}
	t_str = ft_splitter(cmd_str, ' ');
	if (!t_str)
		return (NULL);
	if (fill_command_tokens(cmd, t_str, &decide) < 0)
		return (NULL);
	free_str_array(t_str);
	return (cmd);
}

t_exec	*allocate_shell_commands(int num_commands, char **shell_command)
{
	t_exec	*shell;
	int		i;

	shell = malloc(sizeof(t_exec));
	if (!shell)
		return (NULL);
	shell->commands = malloc(sizeof(t_command *) * (num_commands + 1));
	if (!shell->commands)
	{
		free(shell);
		return (NULL);
	}
	shell->command_count = num_commands;
	i = 0;
	while (i < num_commands)
	{
		shell->commands[i - 1] = allocate_command(shell_command[i], &i);
		if (!shell->commands[i - 1])
		{
			free_shell(shell);
			return (NULL);
		}
	}
	shell->commands[i] = NULL;
	return (shell);
}

void	print_shell(t_exec *shell)
{
	int	i;
	int	j;

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
