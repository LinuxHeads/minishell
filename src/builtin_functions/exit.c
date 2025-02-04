/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:30:45 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/04 20:20:06 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int ft_exit(char **args, t_shell *shell)
{
    if (args[0] && args[1])
    {
        ft_putstr_fd("exit: too many arguments\n", 2);
        return (1);
    }
    if (args[0])
    {
        if (ft_isnumber(args[0]))
        {
            shell->exit_status = (char)ft_atoi(args[0]);
            free_envp_list(shell->env_list);
            free_str_array(shell->envp);
            free(shell->parser);
            exit(shell->exit_status);
            return (0);
        }
        else
        {
            ft_putstr_fd("exit: numeric argument required\n", 2);
            return (2);
        }
    }
    free_envp_list(shell->env_list);
    free_str_array(shell->envp);
    free(shell->parser);
    exit(shell->exit_status);
    return (0);
}