/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:42:42 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/04 04:21:58 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int    ft_env(char **args, t_shell *shell)
{
    if (args[0])
    {
        ft_putstr_fd("env: ", 2);
        ft_putstr_fd("too many args", 2);
        ft_putstr_fd("\n", 2);
        return (1);
    }
    print_envp(shell->env_list);
    // shell->exit_status = 0;
    return (0);
}
