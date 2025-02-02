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

#include "../include/parsing.h"

static void	free_argv(char **argv)
{
	int	i = 0;

	if (!argv)
		return ;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

static void	free_str_array(char **arr)
{
	int	i = 0;
	if (!arr)
		return;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static char	**build_command_argv(t_command *cmd)
{
	int		i;
	int		count;
	char	**argv;

	if (!cmd)
		return (NULL);
	count = 0;
	i = 0;
	while (i < cmd->token_count)
	{
		if (cmd->tokens[i] && (cmd->tokens[i]->type == REDIRECT_IN ||
			cmd->tokens[i]->type == REDIRECT_OUT ||
			cmd->tokens[i]->type == REDIRECT_APPEND ||
			cmd->tokens[i]->type == HEREDOC))
		{
			i += 2;
			continue ;
		}
		count++;
		i++;
	}
	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	i = 0;
	count = 0;
	while (i < cmd->token_count)
	{
		if (cmd->tokens[i] && (cmd->tokens[i]->type == REDIRECT_IN ||
			cmd->tokens[i]->type == REDIRECT_OUT ||
			cmd->tokens[i]->type == REDIRECT_APPEND ||
			cmd->tokens[i]->type == HEREDOC))
		{
			i += 2;
			continue ;
		}
		argv[count] = ft_strdup(cmd->tokens[i]->value);
		if (!argv[count])
		{
			free_argv(argv);
			return (NULL);
		}
		count++;
		i++;
	}
	argv[count] = NULL;
	return (argv);
}

static void	get_redirections(t_command *cmd, int *in_fd, int *out_fd)
{
	int	i = 0;

	if (!cmd || !in_fd || !out_fd)
		return ;
	*in_fd = STDIN_FILENO;
	*out_fd = STDOUT_FILENO;
	while (i < cmd->token_count)
	{
		if (cmd->tokens[i] == NULL)
		{
			i++;
			continue ;
		}
		if (cmd->tokens[i]->type == REDIRECT_IN)
		{
			if (i + 1 < cmd->token_count && cmd->tokens[i + 1])
			{
				i++;
				*in_fd = open(cmd->tokens[i]->value, O_RDONLY);
				if (*in_fd < 0)
					perror(cmd->tokens[i]->value);
			}
			else
				fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
		}
		else if (cmd->tokens[i]->type == REDIRECT_OUT)
		{
			if (i + 1 < cmd->token_count && cmd->tokens[i + 1])
			{
				i++;
				*out_fd = open(cmd->tokens[i]->value,
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (*out_fd < 0)
					perror(cmd->tokens[i]->value);
			}
			else
				fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
		}
		else if (cmd->tokens[i]->type == REDIRECT_APPEND)
		{
			if (i + 1 < cmd->token_count && cmd->tokens[i + 1])
			{
				i++;
				*out_fd = open(cmd->tokens[i]->value,
					O_WRONLY | O_CREAT | O_APPEND, 0644);
				if (*out_fd < 0)
					perror(cmd->tokens[i]->value);
			}
			else
				fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
		}
		else if (cmd->tokens[i]->type == HEREDOC)
		{
			if (i + 1 < cmd->token_count)
			{
				i++;
				int pipe_fds[2];
				if (pipe(pipe_fds) == -1)
				{
					perror("pipe");
					exit(EXIT_FAILURE);
				}
				char *line;
				while (1)
				{
					line = readline("> ");
					if (!line || ft_strcmp(line, cmd->tokens[i]->value) == 0)
					{
						free(line);
						break;
					}
					write(pipe_fds[1], line, ft_strlen(line));
					write(pipe_fds[1], "\n", 1);
					free(line);
				}
				close(pipe_fds[1]);
				*in_fd = pipe_fds[0];
			}
		}
		i++;
	}
}

static char	*find_command_path(char *cmd, char **envp)
{
	char	*path_var;
	char	**paths;
	char	*full_path;
	int		i;

	if (!cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_var = envp[i] + 5;
			paths = ft_split(path_var, ':');
			if (!paths)
				return (NULL);
			i = 0;
			while (paths[i])
			{
				full_path = malloc(ft_strlen(paths[i]) +
						ft_strlen(cmd) + 2);
				if (!full_path)
					break ;
				sprintf(full_path, "%s/%s", paths[i], cmd);
				if (access(full_path, X_OK) == 0)
				{
					free_str_array(paths);
					return (full_path);
				}
				free(full_path);
				i++;
			}
			free_str_array(paths);
			return (NULL);
		}
		i++;
	}
	return (NULL);
}

static void	execute_pipeline(t_shell *shell, char **envp)
{
	int		i;
	int		prev_fd;
	int		pipe_fd[2];
	pid_t	pid;
	int		in_fd;
	int		out_fd;
	char	**argv;
	char	*cmd_path;

	prev_fd = -1;
	i = 0;
	while (i < shell->command_count)
	{
		get_redirections(shell->commands[i], &in_fd, &out_fd);
		if (i > 0 && in_fd == STDIN_FILENO)
			in_fd = prev_fd;
		if (i < shell->command_count - 1 && out_fd == STDOUT_FILENO)
		{
			if (pipe(pipe_fd) == -1)
			{
				perror("pipe");
				exit(EXIT_FAILURE);
			}
			out_fd = pipe_fd[1];
		}
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			if (in_fd != STDIN_FILENO)
				dup2(in_fd, STDIN_FILENO);
			if (out_fd != STDOUT_FILENO)
				dup2(out_fd, STDOUT_FILENO);
			if (prev_fd != -1)
				close(prev_fd);
			if (i < shell->command_count - 1)
				close(pipe_fd[0]);
			argv = build_command_argv(shell->commands[i]);
			if (!argv || !argv[0])
			{
				fprintf(stderr, "minishell: invalid command\n");
				exit(EXIT_FAILURE);
			}
			cmd_path = find_command_path(argv[0], envp);
			if (!cmd_path)
			{
				fprintf(stderr, "%s: command not found\n", argv[0]);
				free_argv(argv);
				exit(127);
			}
			execve(cmd_path, argv, envp);
			perror("execve");
			free(cmd_path);
			free_argv(argv);
			exit(EXIT_FAILURE);
		}
		if (in_fd != STDIN_FILENO && in_fd != prev_fd)
			close(in_fd);
		if (out_fd != STDOUT_FILENO)
			close(out_fd);
		if (prev_fd != -1 && prev_fd != STDIN_FILENO)
			close(prev_fd);
		if (i < shell->command_count - 1 && out_fd == pipe_fd[1])
			prev_fd = pipe_fd[0];
		else
			prev_fd = -1;
		i++;
	}
	while (wait(NULL) > 0)
		;
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	char	*processed_input;
	char	**commands;
	int		num_commands;
	t_shell	*shell;

	(void)argc;
	(void)argv;
	while (1)
	{
		input = readline("\033[1;32mminishell>\033[0m ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		processed_input = preprocess_input(input);
		if (!processed_input)
		{
			free(input);
			continue ;
		}
		commands = ft_split(processed_input, '|');
		if (!commands)
		{
			free(processed_input);
			free(input);
			continue ;
		}
		num_commands = count_words(processed_input, '|');
		shell = allocate_shell_commands(num_commands, commands);
		if (!shell)
		{
			free_str_array(commands);
			free(processed_input);
			free(input);
			continue ;
		}
		execute_pipeline(shell, envp);
		free_shell(shell);
		free_str_array(commands);
		free(processed_input);
		free(input);
	}
	clear_history();
	return (0);
}