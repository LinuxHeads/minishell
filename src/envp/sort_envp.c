/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_envp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 02:31:15 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/11 08:06:23 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_sort_env(t_env **head)
{
	t_env	*sorted;
	t_env	*current;
	t_env	*next;
	t_env	*tmp;

	sorted = NULL;
	current = *head;
	while (current)
	{
		next = current->next;
		if (!sorted || ft_strcmp(current->name, sorted->name) < 0)
		{
			current->next = sorted;
			sorted = current;
		}
		else
		{
			tmp = sorted;
			while (tmp->next && ft_strcmp(tmp->next->name, current->name) < 0)
				tmp = tmp->next;
			current->next = tmp->next;
			tmp->next = current;
		}
		current = next;
	}
	*head = sorted;
}
