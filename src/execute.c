/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 01:23:07 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/04 20:06:18 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int is_redirect_operator(int type) {
    return (type == REDIRECT_IN ||
            type == REDIRECT_OUT ||
            type == REDIRECT_APPEND ||
            type == HEREDOC);
}
static char **build_command_argv(t_command *cmd) 
{
    int i = 0, j = 0, count = 0;
    while (i < cmd->token_count)
	{
        if (is_redirect_operator(cmd->tokens[i]->type))
            i += 2; 
        else {
            count++;
            i++;
        }
    }
    char **argv = malloc(sizeof(char *) * (count + 1));
    if (!argv) return NULL;
    i = 0;
    j = 0;
    while (i < cmd->token_count)
	{
        if (is_redirect_operator(cmd->tokens[i]->type)) {
            i += 2;
            continue;
        }
        argv[j] = ft_strdup(cmd->tokens[i]->value);
        if (!argv[j]) {
            free_str_array(argv);
            return NULL;
        }
        j++;
        i++;
    }
    argv[j] = NULL;
    return argv;
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
		if (cmd->tokens[i]->type == HEREDOC)
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


void	free_str_array(char **arr)
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

static char *find_executable_in_paths(char **cmd, char **paths, char **full_path, int *i)
{
    while (paths[*i])
    {
        *full_path = malloc(ft_strlen(paths[*i]) + ft_strlen(*cmd) + 2);
        if (!*full_path)
            return (NULL);
        sprintf(*full_path, "%s/%s", paths[*i], *cmd);
        if (access(*full_path, X_OK) == 0)
        {
            free_str_array(paths);
            return (*full_path);
        }
        free(*full_path);
        (*i)++;
    }
    return (NULL);
}

static char *find_command_path(char *cmd, char **envp)
{
    char    *path_var;
    char    **paths;
    char    *full_path;
    int     i;

    if (ft_strchr(cmd, '/'))
        return (ft_strdup(cmd));
    i = -1;
    while (envp[++i])
    {
        if (ft_strncmp(envp[i], "PATH=", 5) == 0)
        {
            path_var = envp[i] + 5;
            paths = ft_split(path_var, ':');
            if (!paths)
                return (NULL);
            i = 0;
            full_path = find_executable_in_paths(&cmd, paths, &full_path, &i);
            if (full_path)
                return (full_path);
            free_str_array(paths);
            return (NULL);
        }
    }
    return (NULL);
}
int builtins(char **arg)
{
	if (ft_strcmp(arg[0], "echo") == 0)
		return (1);
	if (ft_strcmp(arg[0], "cd") == 0)
		return (1);
	if (ft_strcmp(arg[0], "pwd") == 0)
		return (1);
	if (ft_strcmp(arg[0], "export") == 0)
		return (1);
	if (ft_strcmp(arg[0], "unset") == 0)
		return (1);
	if (ft_strcmp(arg[0], "env") == 0)
		return (1);
	if (ft_strcmp(arg[0], "exit") == 0)
		return (1);
	return (0);

}

int exec_builtins(char **args, t_shell *shell)
{
	if (ft_strcmp(args[0], "echo") == 0)
		return (1);
	if (ft_strcmp(args[0], "cd") == 0)
		return (ft_cd(args + 1, &shell->env_list));
	if (ft_strcmp(args[0], "pwd") == 0)
		return (ft_pwd(args + 1));
	if (ft_strcmp(args[0], "export") == 0)
		return (ft_export(args + 1, &shell->env_list));
	if (ft_strcmp(args[0], "unset") == 0)
		return (ft_unset(args + 1, &shell->env_list));
	if (ft_strcmp(args[0], "env") == 0)
		return (ft_env(args + 1, shell));
	if (ft_strcmp(args[0], "exit") == 0)
		return (ft_exit(args + 1, shell));
	return (0);

}

void execute_pipeline(t_shell **shell) {
    int i = 0;
    int prev_fd = -1;
    int pipe_fd[2];
    int pid;
    int in_fd, out_fd;
    char **argv;
    char *cmd_path;
    int pipe_created;
    (*shell)->envp = envp_to_str((*shell)->env_list);
    if (!(*shell)->envp || !(*shell)->envp[0]) {
        fprintf(stderr, "Error converting env to string array\n");
        return;
    }
    
    while (i < (*shell)->parser->command_count) {
        get_redirections((*shell)->parser->commands[i], &in_fd, &out_fd);
        argv = build_command_argv((*shell)->parser->commands[i]);
        if (!argv || !argv[0]) {
            fprintf(stderr, "minishell: invalid command\n");
            exit(EXIT_FAILURE);
        }
        if (builtins(argv) && (*shell)->parser->command_count == 1) {
            exec_builtins(argv, *shell);
            free_str_array(argv);
            return;
        } else {
            if (i > 0 && prev_fd != -1)
                in_fd = prev_fd;
            pipe_created = 0;
            if (i < (*shell)->parser->command_count - 1) {
                if (pipe(pipe_fd) == -1) {
                    perror("pipe");
                    exit(EXIT_FAILURE);
                }
                pipe_created = 1;
            }
            pid = fork();
            if (pid < 0) {
                perror("fork");
                exit(EXIT_FAILURE);
            }
            if (pid == 0) {
                if (in_fd != STDIN_FILENO)
                    dup2(in_fd, STDIN_FILENO);
                if (pipe_created) {
                    if (out_fd == STDOUT_FILENO)
                        out_fd = pipe_fd[1];
                }
                if (out_fd != STDOUT_FILENO)
                    dup2(out_fd, STDOUT_FILENO);
                if (prev_fd != -1)
                    close(prev_fd);
                if (pipe_created) {
                    close(pipe_fd[0]);
                    close(pipe_fd[1]);
                }
                argv = build_command_argv((*shell)->parser->commands[i]);
                if (!argv || !argv[0]) {
                    fprintf(stderr, "minishell: invalid command\n");
                    exit(EXIT_FAILURE);
                }
                cmd_path = find_command_path(argv[0], (*shell)->envp);
                if (!cmd_path) {
                    fprintf(stderr, "%s: command not found\n", argv[0]);
                    free_str_array(argv);
                    exit(127);
                }
                execve(cmd_path, argv, (*shell)->envp);
                perror("execve");
                free_str_array(argv);
                free(cmd_path);
                exit(EXIT_FAILURE);
            }
            if (in_fd != STDIN_FILENO && in_fd != prev_fd)
                close(in_fd);
            if (out_fd != STDOUT_FILENO)
                close(out_fd);
            if (prev_fd != -1)
                close(prev_fd);
            if (pipe_created) {
                close(pipe_fd[1]);
                prev_fd = pipe_fd[0];
            } else {
                prev_fd = -1;
            }
        }
        i++;
    }
    int wstatus;
    while (wait(&wstatus) > 0) {
        if (WIFEXITED(wstatus))
            (*shell)->exit_status = WEXITSTATUS(wstatus);
        else if (WIFSIGNALED(wstatus))
            (*shell)->exit_status = 128 + WTERMSIG(wstatus);
    }
}