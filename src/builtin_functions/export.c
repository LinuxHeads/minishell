/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@std.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:42:39 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/03 19:21:21 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void ft_export(char *line, t_env **env_list)
{
    char *args;
    int i;
    
    args = ft_strchr(line, '=');
    if (args)
    {
        line[args - line] = '\0';
        ft_setenv(line, args + 1, env_list);
    }
    else
    {
        ft_setenv(line, NULL, env_list);
    }
}
