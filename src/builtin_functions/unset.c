/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:42:37 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/02 00:27:19 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void    ft_unset(char *arg, t_env **envp)// not fucking working
{
    t_env *tmp;
    t_env *prev;

    tmp = *envp;
    prev = NULL;
    while (tmp)
    {
        if (tmp->name && ft_strcmp(tmp->name, arg) == 0)
        {
            if (prev)
                prev->next = tmp->next;
            else
                *envp = tmp->next;
            free(tmp->name);
            free(tmp->value);
            free(tmp);
            return ;
        }
    }
}