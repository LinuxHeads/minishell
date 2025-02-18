/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 05:34:51 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/18 17:13:16 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	contains_command_token(t_command *cmd)
{
	int	i;

	if (!cmd || !cmd->tokens || cmd->token_count == 0)
		return (0);
	i = 0;
	while (i < cmd->token_count)
	{
		if (cmd->tokens[i] && cmd->tokens[i]->type == COMMAND)
			return (1);
		i++;
	}
	return (0);
}

int	setup_command(t_shell **shell, int i, int *status)
{
	*status = 0;
	(*shell)->argv = build_command_argv((*shell)->parser->commands[i]);
	if (!(*shell)->argv)
	{
		(*shell)->exit_status = 1;
		printf("Error building command argv\n");
		return (0);
	}
	if (expander(shell) < 0)
	{
		if (!get_redirections((*shell)->parser->commands[i], &(*shell)->in_fd,
				&(*shell)->out_fd, *shell))
		{
			(*shell)->exit_status = 1;
			*status = 1;
		}
		free_str_array((*shell)->argv);
		close_fds(*shell);
		return (0);
	}
	return (1);
}

int	process_command(t_shell **shell, int i, int *pid)
{
	int	redir_flag;

	redir_flag = 0;
	if (handle_signal_and_token(shell, i, &redir_flag))
		return (1);
	if (!get_redirections((*shell)->parser->commands[i], &(*shell)->in_fd,
			&(*shell)->out_fd, *shell))
	{
		(*shell)->exit_status = 1;
		redir_flag = 1;
	}
	if (!(*shell)->argv || !(*shell)->argv[0])
	{
		free_str_array((*shell)->argv);
		return (2);
	}
	if (is_builtin_command((*shell)->argv)
		&& (*shell)->parser->command_count == 1)
	{
		execute_builtin_in_parent(shell, redir_flag);
		return (2);
	}
	exec_in_child(i, shell, pid, redir_flag);
	free_str_array((*shell)->argv);
	return (0);
}
