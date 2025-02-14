/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 01:23:07 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/14 04:26:12 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	wait_for_children(t_shell *shell, int pid)
{
	int	wstatus;
	int	last_pid;

	last_pid = wait(&wstatus);
	while (last_pid > 0)
	{
		if (last_pid == pid)
		{
			if (WIFEXITED(wstatus))
				shell->exit_status = WEXITSTATUS(wstatus);
			else if (WIFSIGNALED(wstatus))
				shell->exit_status = 128 + WTERMSIG(wstatus);
		}
		last_pid = wait(&wstatus);
	}
}

void	free_str_array(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static int	has_valid_command(t_command *cmd)
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

void	execute_pipeline(t_shell **shell)
{
	int	i;
	int	pid;
	int	redir_flag;

	i = 0;
	(*shell)->prev_fd = -1;
	if ((*shell)->envp)
		free_str_array((*shell)->envp);
	(*shell)->envp = envp_to_str((*shell)->env_list);
	if (!(*shell)->envp || !(*shell)->envp[0])
	{
		fprintf(stderr, "Error converting env to string array\n");
		return ;
	}
	signals_setup(1);
	while (i < (*shell)->parser->command_count)
	{
		(*shell)->argv = build_command_argv((*shell)->parser->commands[i]);
		if (!(*shell)->argv)
		{
			(*shell)->exit_status = 1;
			printf("Error building command argv\n");
			i++;
			continue ;
		}
		if (!expander(shell))
		{
			if (!get_redirections((*shell)->parser->commands[i],
					&(*shell)->in_fd, &(*shell)->out_fd, *shell))
				// found leaks caused by preprocess_input_test
			{
				(*shell)->exit_status = 1;
				redir_flag = 1;
			}
			i++;
			continue ;
		}
		redir_flag = 0;
		if (!get_redirections((*shell)->parser->commands[i], &(*shell)->in_fd,
				&(*shell)->out_fd, *shell))
		{
			(*shell)->exit_status = 1;
			redir_flag = 1;
		}
		if (g_signal_flag == SIGINT)
		{
			g_signal_flag = 0;
			i++;
			(*shell)->exit_status = 130;
			continue ;
		}
		if (!has_valid_command((*shell)->parser->commands[i]))
		{
			if (!get_redirections((*shell)->parser->commands[i],
					&(*shell)->in_fd, &(*shell)->out_fd, *shell))
			{
				(*shell)->exit_status = 1;
				redir_flag = 1;
			}
			i++;
			continue ;
		}
		if (!(*shell)->argv || !(*shell)->argv[0])
		{
			fprintf(stderr, "minishell: invalid command\n");
			exit(EXIT_FAILURE);
		}
		if (is_builtin((*shell)->argv) && (*shell)->parser->command_count == 1)
		{
			exec_in_parent(shell, redir_flag);
			return ;
		}
		else
			exec_in_child(i, shell, &pid, redir_flag);
		i++;
	}
	wait_for_children(*shell, pid);
}
