/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_envp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 02:31:15 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/16 13:21:15 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	insert_sorted(t_env **sorted, t_env *current)
{
	t_env	*tmp;

	tmp = *sorted;
	while (tmp->next && ft_strcmp(tmp->next->name, current->name) < 0)
		tmp = tmp->next;
	current->next = tmp->next;
	tmp->next = current;
}

void	ft_sort_env(t_env **head)
{
	t_env	*sorted;
	t_env	*current;
	t_env	*next;

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
			insert_sorted(&sorted, current);
		current = next;
	}
	*head = sorted;
}
