/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_in_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 02:41:05 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/13 23:33:44 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	create_pipe(int *pipe_fd, int i, t_shell **shell)
{
	if (i < (*shell)->parser->command_count - 1)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return (0);
		}
		return (1);
	}
	return (0);
}

void	exit_error(int *in_fd, int *out_fd, int pipe_created, int *pipe_fd)
{
	if (*in_fd != STDIN_FILENO)
		close(*in_fd);
	if (*out_fd != STDOUT_FILENO)
		close(*out_fd);
	if (pipe_created)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	exit(EXIT_FAILURE);
}

void	dir_check(char *cmd_path, char **argv)
{
	struct stat	path_stat;

	if (stat(cmd_path, &path_stat) == 0)
	{
		if (S_ISDIR(path_stat.st_mode))
		{
			if (!ft_strchr(argv[0], '/'))
			{
				fprintf(stderr, "%s: command not found\n", argv[0]);
				free_str_array(argv);
				free(cmd_path);
				exit(127);
			}
			fprintf(stderr, "%s: is a directory\n", cmd_path);
			free_str_array(argv);
			free(cmd_path);
			exit(126);
		}
	}
}

void	check_arg(char **argv)
{
	if (access(argv[0], F_OK) == 0)
	{
		fprintf(stderr, "%s: permission denied\n", argv[0]);
		free_str_array(argv);
		exit(126);
	}
	else
	{
		fprintf(stderr, "%s: command not found\n", argv[0]);
		free_str_array(argv);
		exit(127);
	}
}

void	check_cmd_path(char *cmd_path, char **argv, t_shell **shell)
{
	if (!cmd_path)
	{
		if (ft_strchr(argv[0], '/'))
			check_arg(argv);
		else
		{
			if (ft_strcmp(argv[0], "~") == 0)
			{
				fprintf(stderr, "%s: is a directory\n", ft_getenv("HOME",
						(*shell)->env_list));
				free_str_array(argv);
				free(cmd_path);
				exit(126);
			}
			fprintf(stderr, "%s: command not found\n", argv[0]);
			free_str_array(argv);
			exit(127);
		}
	}
	dir_check(cmd_path, argv);
}

/*
** Helper: child_redirect_fds
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
static void	child_redirect_fds(int *fds, int pipe_created, int pipe_fd[2],
		int prev_fd)
{
	if (fds[0] != STDIN_FILENO)
		if (dup2(fds[0], STDIN_FILENO) == -1)
			perror("dup2");
	if (pipe_created)
	{
		if (fds[1] == STDOUT_FILENO)
			fds[1] = pipe_fd[1];
	}
	if (fds[1] != STDOUT_FILENO)
		if (dup2(fds[1], STDOUT_FILENO) == -1)
			perror("dup2");
	if (prev_fd != -1)
		close(prev_fd);
	if (pipe_created)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
}

/*
** Helper: child_run_command
**
** After setting up redirections, this function:
** - Checks for and executes built-in commands.
** - Locates the command’s executable path.
** - Calls execve to execute the command.
** - If execve returns (i.e. an error occurs), it cleans up and exits
**   with an appropriate exit code.
*/
static void	child_run_command(t_shell **shell, char **argv, int pipe_created,
		int pipe_fd[2], int *in_fd, int *out_fd, int prev_fd)
{
	char	*cmd_path;

	if (is_builtin(argv))
	{
		(*shell)->exit_status = exec_builtins(argv, *shell);
		free_str_array(argv);
		ft_exit_handler(*shell, (int[]){*in_fd, *out_fd, prev_fd}, 3, NULL);
	}
	cmd_path = find_command_path(argv[0], (*shell)->envp);
	check_cmd_path(cmd_path, argv, shell);
	execve(cmd_path, argv, (*shell)->envp);
	perror("execve");
	free_str_array(argv);
	if (pipe_created)
		close(pipe_fd[1]);
	(*shell)->exit_status = 127;
	if (errno == ENOENT)
		ft_exit_handler(*shell, (int[]){*in_fd, *out_fd, prev_fd}, 3, cmd_path);
	(*shell)->exit_status = 127;
	if (errno == EACCES || errno == EISDIR)
		ft_exit_handler(*shell, NULL, 0, cmd_path);
	(*shell)->exit_status = 1;
	ft_exit_handler(*shell, (int[]){*in_fd, *out_fd, prev_fd}, 3, cmd_path);
}

/*
** Helper: parent_cleanup
**
** In the parent process (after fork), cleans up unused file descriptors.
** - Closes in_fd if it is not the standard input or the previous pipe.
** - Closes out_fd if it is not the standard output.
** - Closes the previous pipe file descriptor.
** - If a new pipe was created,
	closes its write end and updates prev_fd to its read end.
**   Otherwise, sets prev_fd to -1.
*/
static void	parent_cleanup(int *in_fd, int *out_fd, int *prev_fd,
		int pipe_created, int pipe_fd[2])
{
	if (*in_fd != STDIN_FILENO && *in_fd != *prev_fd)
		close(*in_fd);
	if (*out_fd != STDOUT_FILENO)
		close(*out_fd);
	if (*prev_fd != -1)
		close(*prev_fd);
	if (pipe_created)
	{
		close(pipe_fd[1]);
		*prev_fd = pipe_fd[0];
	}
	else
	{
		*prev_fd = -1;
	}
}

/*
** Main Function: exec_in_child
**
** This function forks a child process to execute a command (or part of a pipeline).
** It sets up the proper file descriptors (handling pipes and redirections),
	forks,
** and then in the child:
**  - Resets signals.
**  - Validates the command.
**  - Sets up I/O redirections.
**  - Executes the command (or built-in).
** In the parent,
	it cleans up file descriptors and prepares for the next command in the pipeline.
*/
void	exec_in_child(int i, t_shell **shell, int *pid, int *in_fd, int *out_fd,
		char **argv, int redir_flag, int *prev_fd)
{
	int	pipe_fd[2];
	int	pipe_created;

	if (i > 0 && *prev_fd != -1 && *in_fd == STDIN_FILENO)
		*in_fd = *prev_fd;
	pipe_created = create_pipe(pipe_fd, i, shell);
	*pid = fork();
	if (*pid < 0)
	{
		perror("fork");
		if (argv)
			free_str_array(argv);
		if (pipe_created)
		{
			close(pipe_fd[0]);
			close(pipe_fd[1]);
		}
		ft_exit_handler(*shell, (int[]){*in_fd, *out_fd, *prev_fd}, 3, NULL);
	}
	if (*pid == 0)
	{
		reset_signals();
		if (!argv || !argv[0])
		{
			fprintf(stderr, "minishell: invalid command\n");
			if (pipe_created)
			{
				close(pipe_fd[0]);
				close(pipe_fd[1]);
			}
			ft_exit_handler(*shell, (int[]){*in_fd, *out_fd, *prev_fd}, 3,
				NULL);
		}
		if (redir_flag)
		{
			if (pipe_created)
			{
				close(pipe_fd[0]);
				close(pipe_fd[1]);
			}
			free_str_array(argv);
			ft_exit_handler(*shell, (int[]){*in_fd, *out_fd, *prev_fd}, 3,
				NULL);
		}
		child_redirect_fds((int[]){*in_fd, *out_fd}, pipe_created, pipe_fd,
			*prev_fd);
		child_run_command(shell, argv, pipe_created, pipe_fd, in_fd, out_fd,
			*prev_fd);
	}
	parent_cleanup(in_fd, out_fd, prev_fd, pipe_created, pipe_fd);
}
