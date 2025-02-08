/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_envp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 02:31:15 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/08 05:03:15 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
 * Bubble sort by rearranging pointers.
 * Ensures that after each pass, the highest (lexicographically) environment name
 * is placed at the end of the list.
 */
void ft_sort_env(t_env **head)
{
    if (!head || !(*head))
        return;

    int len = ft_lstsize_env(*head);

    for (int i = 0; i < len - 1; i++)
    {
        t_env *current = *head;
        t_env *prev = NULL;
        t_env *next = NULL;
        int swapped = 0;

        /* In each pass, move the largest "name" toward the end. */
        for (int j = 0; j < len - i - 1; j++)
        {
            if (!current->next)
                break;
            next = current->next;

            if (ft_strcmp(current->name, next->name) > 0)
            {
                swapped = 1;
                /* Re-link pointers to swap the entire nodes. */
                current->next = next->next;
                next->next = current;

                if (prev == NULL)
                {
                    /* Update the head if swapping the first node. */
                    *head = next;
                }
                else
                {
                    prev->next = next;
                }
                /* After swap, 'next' is now the node at 'prev->next'. */
                prev = next;
            }
            else
            {
                /* Move forward. */
                prev = current;
                current = current->next;
            }
        }
        /* If nothing was swapped this pass, list is sorted. */
        if (!swapped)
            break;
    }
}
