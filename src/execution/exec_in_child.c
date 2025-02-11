/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_in_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 02:41:05 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/11 05:06:20 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int create_pipe(int *pipe_fd, int i, t_shell **shell)
{
    if (i < (*shell)->parser->command_count - 1)
    {
        if (pipe(pipe_fd) == -1)
        {
            perror("pipe");
            return (0);
        }
    }
    return (1);
}

void    exit_error(int *in_fd, int *out_fd, int pipe_created, int *pipe_fd)
{
    if (*in_fd != STDIN_FILENO)
        close(*in_fd);
    if (*out_fd != STDOUT_FILENO)
        close(*out_fd);
    if (pipe_created) {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
    }
    exit(EXIT_FAILURE);
}

void exec_in_child(int i, t_shell **shell, int *pid, int *in_fd, int *out_fd, char **argv, int redir_flag, int *prev_fd)
{
    int pipe_fd[2];
    int pipe_created;
    char *cmd_path;
    
    if (i > 0 && *prev_fd != -1 && *in_fd == STDIN_FILENO)
    {
        *in_fd = *prev_fd;
    }
    pipe_created = create_pipe(pipe_fd, i, shell);
    *pid = fork();
    if (*pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (*pid == 0)
    {
        reset_signals();
        if (!argv || !argv[0])
        {
            fprintf(stderr, "minishell: invalid command\n");
            exit(EXIT_FAILURE);
        }
        if (redir_flag)
            exit_error(in_fd, out_fd, pipe_created, pipe_fd);
        if (*in_fd != STDIN_FILENO)
            dup2(*in_fd, STDIN_FILENO);
        if (pipe_created) {
            if (*out_fd == STDOUT_FILENO)
                *out_fd = pipe_fd[1];
        }
        if (*out_fd != STDOUT_FILENO)
            dup2(*out_fd, STDOUT_FILENO);
        if (*prev_fd != -1)
            close(*prev_fd);
        if (pipe_created) {
            close(pipe_fd[0]);
            close(pipe_fd[1]);
        }
        
        if (is_builtin(argv))
        {
            (*shell)->exit_status = exec_builtins(argv, *shell);
            free_str_array(argv);
            exit((*shell)->exit_status);
        }
        cmd_path = find_command_path(argv[0], (*shell)->envp);
        if (!cmd_path)
        {
            fprintf(stderr, "%s: command not found\n", argv[0]);
            free_str_array(argv);
            exit(127);
        }
        struct stat path_stat;
        if (stat(cmd_path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
        {
            fprintf(stderr, "%s: is a directory\n", cmd_path);
            free_str_array(argv);
            free(cmd_path);
            exit(126);
        }
        execve(cmd_path, argv, (*shell)->envp);
        perror("execve");
        free_str_array(argv);
        free(cmd_path);
        if (pipe_created)
            close(pipe_fd[1]);
        if (*in_fd != STDIN_FILENO)
            close(*in_fd);
        if (*out_fd != STDOUT_FILENO)
            close(*out_fd);
        if (*prev_fd != -1)
            close(*prev_fd);
        if (errno == EACCES || errno == EISDIR)
            exit(126);
        if (errno == ENOENT)
            exit(127);
        exit(EXIT_FAILURE);
    }
    if (*in_fd != STDIN_FILENO && *in_fd != *prev_fd)
        close(*in_fd);
    if (*out_fd != STDOUT_FILENO)
        close(*out_fd);
    if (*prev_fd != -1)
        close(*prev_fd);
    if (pipe_created) {
        close(pipe_fd[1]);
        *prev_fd = pipe_fd[0];
    } else {
        *prev_fd = -1;
    }
}
