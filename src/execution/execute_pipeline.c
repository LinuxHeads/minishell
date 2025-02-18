/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 01:23:07 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/18 17:12:19 by ahramada         ###   ########.fr       */
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

static int	init_pipeline(t_shell **shell)
{
	(*shell)->prev_fd = -1;
	if ((*shell)->envp)
		free_str_array((*shell)->envp);
	(*shell)->envp = envp_to_str((*shell)->env_list);
	if (!(*shell)->envp || !(*shell)->envp[0])
	{
		ft_putstr_fd("Error converting env to string array\n", STDERR_FILENO);
		(*shell)->exit_status = 1;
		return (0);
	}
	signals_setup(1);
	return (1);
}

int	handle_signal_and_token(t_shell **shell, int i, int *redir_flag)
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
