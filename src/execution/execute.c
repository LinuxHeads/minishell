/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 01:23:07 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/16 18:16:55 by ahramada         ###   ########.fr       */
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

static int	contains_command_token(t_command *cmd)
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

static int	init_pipeline(t_shell **shell)
{
	(*shell)->prev_fd = -1;
	if ((*shell)->envp)
		free_str_array((*shell)->envp);
	(*shell)->envp = envp_to_str((*shell)->env_list);
	if (!(*shell)->envp || !(*shell)->envp[0])
	{
		fprintf(stderr, "Error converting env to string array\n");
		return (0);
	}
	signals_setup(1);
	return (1);
}

static void	close_fds(t_shell *shell)
{
	if (shell->in_fd != STDIN_FILENO)
		close(shell->in_fd);
	if (shell->out_fd != STDOUT_FILENO)
		close(shell->out_fd);
}

static int	setup_command(t_shell **shell, int i, int *status)
{
	*status = 0;
	(*shell)->argv = build_command_argv((*shell)->parser->commands[i]);
	if (!(*shell)->argv)
	{
		(*shell)->exit_status = 1;
		printf("Error building command argv\n");
		return (0);
	}
	if (!expander(shell))
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

static int	handle_signal_and_token(t_shell **shell, int i, int *redir_flag)
{
	if (g_signal_flag == SIGINT)
	{
		g_signal_flag = 0;
		(*shell)->exit_status = 130;
		free_str_array((*shell)->argv);
		return (1);
	}
	if (!contains_command_token((*shell)->parser->commands[i]))
	{
		if (!get_redirections((*shell)->parser->commands[i], &(*shell)->in_fd,
				&(*shell)->out_fd, *shell))
		{
			(*shell)->exit_status = 1;
			*redir_flag = 1;
		}
		free_str_array((*shell)->argv);
		return (1);
	}
	return (0);
}

static int	process_command(t_shell **shell, int i, int *pid)
{
	int	redir_flag;

	redir_flag = 0;
	if (!get_redirections((*shell)->parser->commands[i], &(*shell)->in_fd,
			&(*shell)->out_fd, *shell))
	{
		(*shell)->exit_status = 1;
		redir_flag = 1;
	}
	if (handle_signal_and_token(shell, i, &redir_flag))
		return (1);
	if (!(*shell)->argv || !(*shell)->argv[0])
	{
		fprintf(stderr, "minishell: invalid command\n");
		ft_exit_handler(*shell, NULL, NULL, (*shell)->exit_status);
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

void	execute_pipeline(t_shell **shell)
{
	int	i;
	int	pid;
	int	status;

	i = 0;
	if (!init_pipeline(shell))
		return ;
	while (i < (*shell)->parser->command_count)
	{
		if (!setup_command(shell, i, &status))
		{
			i++;
			continue ;
		}
		status = process_command(shell, i, &pid);
		if (status == 2)
			return ;
		i++;
	}
	wait_for_children(*shell, pid);
}
