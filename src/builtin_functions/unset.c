/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:42:37 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/01 23:47:21 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void    ft_unset(char *arg, t_shell *shell)
{
    t_env *tmp;
    t_env *prev;

    tmp = shell->env_list;
    prev = NULL;
    while (tmp)
    {
        if (tmp->name && ft_strcmp(tmp->name, arg) == 0)
        {
            if (prev)
                prev->next = tmp->next;
            else
                shell->env_list = tmp->next;
            free(tmp->name);
            free(tmp->value);
            free(tmp);
            return ;
        }
    }
}