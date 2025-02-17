/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_in_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 02:41:05 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/17 15:57:30 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <string.h>

int	setup_pipe_for_command(int i, t_shell **shell)
{
	if (i < (*shell)->parser->command_count - 1)
	{
		if (pipe((*shell)->pipe_fd) == -1)
		{
			perror("pipe");
			return (0);
		}
		return (1);
	}
	return (0);
}

/*
** Helper: setup_fd_redirection
**
** Sets up file descriptor redirections for the child process.
** - Duplicates in_fd to STDIN if needed.
** - If a pipe was created,
	uses its write end as STDOUT if out_fd was not redirected.
** - Duplicates out_fd to STDOUT.
**
	- Closes the previous file descriptor (if any)
	and both pipe ends (since they are
**   no longer needed after dup2).
*/
static void	setup_fd_redirection(t_shell **shell)
{
	if ((*shell)->in_fd != STDIN_FILENO)
	{
		if (dup2((*shell)->in_fd, STDIN_FILENO) == -1)
			perror("dup2");
		close((*shell)->in_fd);
	}
	if ((*shell)->pipe_created)
	{
		if ((*shell)->out_fd == STDOUT_FILENO)
			(*shell)->out_fd = (*shell)->pipe_fd[1];
	}
	if ((*shell)->out_fd != STDOUT_FILENO)
	{
		if (dup2((*shell)->out_fd, STDOUT_FILENO) == -1)
			perror("dup2");
		close((*shell)->out_fd);
	}
	if ((*shell)->prev_fd != -1)
		close((*shell)->prev_fd);
	if ((*shell)->pipe_created)
	{
		close((*shell)->pipe_fd[0]);
		close((*shell)->pipe_fd[1]);
	}
}

/*
** Helper: execute_command
**
** After setting up redirections, this function:
** - Checks for and executes built-in commands.
** - Locates the command’s executable path.
** - Calls execve to execute the command.
** - If execve returns (i.e. an error occurs), it cleans up and exits
**   with an appropriate exit code.
*/
static void	execute_command(t_shell **shell)
{
	char	*cmd_path;

	if (is_builtin_command((*shell)->argv))
	{
		(*shell)->exit_status = execute_builtin_command((*shell)->argv, *shell);
		ft_exit_handler(*shell, NULL, 0, (*shell)->exit_status);
	}
	cmd_path = find_command_path((*shell)->argv[0], (*shell)->envp);
	validate_command_path(cmd_path, shell);
	execve(cmd_path, (*shell)->argv, (*shell)->envp);
	perror("execve");
	if ((*shell)->pipe_created)
		close((*shell)->pipe_fd[1]);
	(*shell)->exit_status = 127;
	if (errno == ENOENT)
		ft_exit_handler(*shell, cmd_path, 0, 127);
	(*shell)->exit_status = 127;
	if (errno == EACCES || errno == EISDIR)
		ft_exit_handler(*shell, cmd_path, 0, 126);
	(*shell)->exit_status = 1;
	ft_exit_handler(*shell, cmd_path, 0, 1);
}

/*
** Helper: cleanup_parent_fds
**
** In the parent process (after fork), cleans up unused file descriptors.
** - Closes in_fd if it is not the standard input or the previous pipe.
** - Closes out_fd if it is not the standard output.
** - Closes the previous pipe file descriptor.
** - If a new pipe was created,
	closes its write end and updates prev_fd to its read end.
**   Otherwise, sets prev_fd to -1.
*/
static void	cleanup_parent_fds(t_shell **shell)
{
	if ((*shell)->in_fd != STDIN_FILENO && (*shell)->in_fd != (*shell)->prev_fd)
	{
		close((*shell)->in_fd);
		(*shell)->in_fd = STDIN_FILENO;
	}
	if ((*shell)->out_fd != STDOUT_FILENO)
	{
		close((*shell)->out_fd);
		(*shell)->out_fd = STDOUT_FILENO;
	}
	if ((*shell)->prev_fd != -1)
	{
		close((*shell)->prev_fd);
		(*shell)->prev_fd = -1;
	}
	if ((*shell)->pipe_created)
	{
		close((*shell)->pipe_fd[1]);
		(*shell)->prev_fd = (*shell)->pipe_fd[0];
		(*shell)->pipe_created = 0;
	}
}

/*
** Main Function: exec_in_child
**
** This function forks a child process to execute
	a command (or part of a pipeline).
** It sets up the proper file descriptors (handling pipes and redirections),
	forks,
** and then in the child:
**  - Resets signals.
**  - Validates the command.
**  - Sets up I/O redirections.
**  - Executes the command (or built-in).
** In the parent,
	it cleans up file descriptors and prepares
	for the next command in the pipeline.
*/

void	exec_in_child(int i, t_shell **shell, int *pid, int redir_flag)
{
	if (i > 0 && (*shell)->prev_fd != -1 && (*shell)->in_fd == STDIN_FILENO)
		(*shell)->in_fd = (*shell)->prev_fd;
	(*shell)->pipe_created = setup_pipe_for_command(i, shell);
	*pid = fork();
	fork_check(*pid, shell);
	if (*pid == 0)
	{
		reset_signals();
		if (!(*shell)->argv || !(*shell)->argv[0])
			ft_exit_handler(*shell, NULL,
				(char *[]){"minishell: invalid command\n", NULL}, 1);
		if (redir_flag)
			ft_exit_handler(*shell, NULL, 0, 1);
		setup_fd_redirection(shell);
		execute_command(shell);
	}
	cleanup_parent_fds(shell);
}
