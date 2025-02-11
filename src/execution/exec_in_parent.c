/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_in_parent.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 04:00:59 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/11 04:53:58 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void exec_in_parent(int in_fd, int out_fd, t_shell **shell, char **argv, int redir_flag)
{
    int fd1 = -1;
    
    if (redir_flag)
    {
        if (in_fd != STDIN_FILENO)
            close(in_fd);
        if (out_fd != STDOUT_FILENO)
            close(out_fd);
        return ;
    }
    if (out_fd != STDOUT_FILENO)
    {
        fd1 = dup(STDOUT_FILENO);
        if (fd1 < 0)
        {
            perror("dup");
            close(out_fd);
            return ;
        }
        if (dup2(out_fd, STDOUT_FILENO) < 0)
        {
            close(fd1);
            close(out_fd);
            perror("dup2");
            return ;
        }
    }
    (*shell)->exit_status = exec_builtins(argv, *shell);
    free_str_array(argv);
    if (out_fd != STDOUT_FILENO)
    {
        if (dup2(fd1, STDOUT_FILENO) < 0)
        {
            close(fd1);
            perror("dup2");
        }
        close(fd1);
        close(out_fd);
    }
}

