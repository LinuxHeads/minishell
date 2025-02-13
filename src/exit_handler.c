/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 20:19:38 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/13 22:47:10 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


void    ft_exit_handler(t_shell *shell, int *fds, int count, void *ptr)
{
    int i;
    
    i = 0;
    while (i < count)
    {
        if (fds[i] != -1 && fds[i] != STDIN_FILENO && fds[i] != STDOUT_FILENO)
            close(fds[i]);
        i++;
    }
    if (shell)
    {
        free_envp_list(shell->env_list);
        free_str_array(shell->argv);
        free_envp_array(shell->envp);
        free_shell(shell->parser);
    }
    if (ptr)
        free(ptr);
    exit(shell->exit_status);
}
