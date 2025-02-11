/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_in_parent.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 04:00:59 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/11 23:02:18 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	redir_in_parent(int out_fd, int *fd1)
{
	if (out_fd != STDOUT_FILENO)
	{
		*fd1 = dup(STDOUT_FILENO);
		if (*fd1 < 0)
		{
			perror("dup");
			close(out_fd);
			return (0);
		}
		if (dup2(out_fd, STDOUT_FILENO) < 0)
		{
			close(*fd1);
			close(out_fd);
			perror("dup2");
			return (0);
		}
	}
	return (1);
}

void	exec_in_parent(int *fds, t_shell **shell, char **argv,
		int redir_flag)
{
	int	fd1;

	fd1 = -1;
	if (redir_flag)
	{
		if (fds[0] != STDIN_FILENO)
			close(fds[0]);
		if (fds[1] != STDOUT_FILENO)
			close(fds[1]);
		return ;
	}
	if (fds[1] != STDOUT_FILENO)
		if (!redir_in_parent(fds[1], &fd1))
			return ;
	(*shell)->exit_status = exec_builtins(argv, *shell);
	free_str_array(argv);
	if (fds[1] != STDOUT_FILENO)
	{
		if (dup2(fd1, STDOUT_FILENO) < 0)
			perror("dup2");
		close(fd1);
		close(fds[1]);
	}
}
