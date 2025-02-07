/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 20:19:38 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/07 03:50:40 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void ft_free_commands(t_exec *exec)
{
    int i;

    i = 0;
    while (exec->commands[i])
    {
        free_str_array(exec->commands[i]->tokens);
        free(exec->commands[i]);
        i++;
    }
    free(exec->commands);
    free(exec);
}

void    ft_exit_handler(t_shell *shell, int *fds, int count)
{
    int i;
    
    i = 0;
    while (i < count)
    {
        if (fds[i] != -1)
            close(fds[i]);
        i++;
    }
    free_envp_list(shell->env_list);
    free_envp_array(shell->envp);
    ft_free_commands(shell->parser);
}