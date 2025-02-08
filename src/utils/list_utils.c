/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 02:09:47 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/08 02:26:39 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int    ft_lstsize_env(t_env *env_list)
{
    int		size;
    t_env	*tmp;

    size = 0;
    tmp = env_list;
    while (tmp)
    {
        size++;
        tmp = tmp->next;
    }
    return (size);
}

