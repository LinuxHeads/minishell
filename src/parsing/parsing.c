/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yourlogin <yourlogin@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 01:23:07 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/02 15:00:00 by yourlogin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* --- Helper to identify redirection tokens --- */

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

/* --- Fills a command’s token array --- */

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

/* --- Allocates a single command --- */

static t_command	*allocate_command(char *cmd_str)
{
	t_command		*cmd;
	t_next_token	decide;
	char			**t_str;

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

/* --- Allocates all shell commands --- */

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
		shell->commands[i] = allocate_command(shell_command[i]);
		if (!shell->commands[i])
			return (NULL);
		i++;
	}
	shell->commands[i] = NULL;
	return (shell);
}
