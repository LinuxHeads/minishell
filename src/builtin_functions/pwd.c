/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 03:24:37 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/05 12:56:48 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int ft_pwd(void*)
{
    char cwd[1024];

    // if (args[0])
    // {
    //     ft_putstr_fd("pwd: too many arguments\n", 2);
    //     return (1);
    // }
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        ft_putendl_fd(cwd, 1);
        return (0);
    }
    else
    {
        perror("getcwd");
        return (1);
    }
}