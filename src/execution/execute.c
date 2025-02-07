/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 01:23:07 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/08 00:46:49 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"



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
static int has_valid_command(t_command *cmd)
{
    int i;
    if (!cmd || !cmd->tokens || cmd->token_count == 0)
        return (0);
    i=0;
    while (i < cmd->token_count)
    {
        if (cmd->tokens[i] && cmd->tokens[i]->type == COMMAND)
            return (1);
        i++;
    }
    return (0);
}

void execute_pipeline(t_shell **shell)
{
    int i = 0;
    int prev_fd = -1;
    int pipe_fd[2];
    int pid;
    int in_fd, out_fd;
    char **argv;
    char *cmd_path;
    int pipe_created;
    int last_pid;
    int wstatus;
    int redir_flag;
    
    
    (*shell)->envp = envp_to_str((*shell)->env_list);
    if (!(*shell)->envp || !(*shell)->envp[0])
    {
        fprintf(stderr, "Error converting env to string array\n");
        return;
    }
    
    while (i < (*shell)->parser->command_count)
    {

        argv = build_command_argv((*shell)->parser->commands[i]);
        if(!expander(&argv, *shell))
        {
            i++;
            continue;
        }
        redir_flag = 0;
        if (!get_redirections((*shell)->parser->commands[i], &in_fd, &out_fd, *shell))
        {
            (*shell)->exit_status = 1;
            redir_flag = 1;
        }
        if (!has_valid_command((*shell)->parser->commands[i]))
        {
            if (!get_redirections((*shell)->parser->commands[i], &in_fd, &out_fd, *shell))
            {
                (*shell)->exit_status = 1;
                redir_flag = 1;
            }
            // close(in_fd);
            // close(out_fd);
            i++;
            continue;
        }
        //printf("%s \n",argv[0]); 
        if (!argv || !argv[0])
        {
            fprintf(stderr, "minishell: invalid command\n");
            exit(EXIT_FAILURE);
        }
        if (is_builtin(argv) && (*shell)->parser->command_count == 1)
        {
            int fd1 = -1;
            int fd2 = -1;
            
            if (redir_flag)
            {
                if (in_fd != STDIN_FILENO)
                    close(in_fd);
                if (out_fd != STDOUT_FILENO)
                    close(out_fd);
                return;
            }
            if (out_fd != STDOUT_FILENO)
            {
                fd1 = dup(STDOUT_FILENO);
                fd2 = dup(STDIN_FILENO);
                dup2(out_fd, STDOUT_FILENO);
            }
            (*shell)->exit_status = exec_builtins(argv, *shell);
            free_str_array(argv);
            if (out_fd != STDOUT_FILENO)
            {
                dup2(fd1, STDOUT_FILENO);
                close(fd1);
                close(fd2);
                close(out_fd);
            }
            return;
        } 
        else
        {
            if (i > 0 && prev_fd != -1 && in_fd == STDIN_FILENO)
                 {in_fd = prev_fd;}
            pipe_created = 0;
            if (i < (*shell)->parser->command_count - 1)
            {
                if (pipe(pipe_fd) == -1) {
                    perror("pipe");
                    exit(EXIT_FAILURE);
                }
                pipe_created = 1;
            }
            pid = fork();
            if (pid < 0)
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }
            if (pid == 0)
            {
                if (redir_flag)
                {
                    if (in_fd != STDIN_FILENO)
                        close(in_fd);
                    if (out_fd != STDOUT_FILENO)
                        close(out_fd);
                    if (pipe_created) {
                     close(pipe_fd[0]);
                     close(pipe_fd[1]);
                    }
                    exit(EXIT_FAILURE);
                }
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
                if (!argv || !argv[0]) {
                    fprintf(stderr, "minishell: invalid command\n");
                    exit(EXIT_FAILURE);
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
                if (in_fd != STDIN_FILENO)
                    close(in_fd);
                if (out_fd != STDOUT_FILENO)
                    close(out_fd);
                if (prev_fd != -1)
                    close(prev_fd);
                if (errno == EACCES || errno == EISDIR)
                    exit(126);
                if (errno == ENOENT)
                    exit(127);
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
    while ((last_pid = wait(&wstatus)) > 0) {
        if (last_pid == pid)
        {
        if (WIFEXITED(wstatus))
            (*shell)->exit_status = WEXITSTATUS(wstatus);
        else if (WIFSIGNALED(wstatus))
            (*shell)->exit_status = 128 + WTERMSIG(wstatus);
    }}
}
