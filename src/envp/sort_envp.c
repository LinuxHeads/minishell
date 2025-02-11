/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_envp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 02:31:15 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/11 07:58:40 by abdsalah         ###   ########.fr       */
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
    if (!head || !(*head) || !(*head)->next)
        return;

    int swapped;
    t_env *current;
    t_env *prev;
    t_env *next;
    t_env *last_sorted = NULL;

    do
    {
        swapped = 0;
        current = *head;
        prev = NULL;

        while (current->next != last_sorted)
        {
            next = current->next;
            if (ft_strcmp(current->name, next->name) > 0)
            {
                swapped = 1;
                /* Swap nodes */
                current->next = next->next;
                next->next = current;

                if (prev == NULL)
                    *head = next;
                else
                    prev->next = next;

                /* Move prev forward */
                prev = next;
            }
            else
            {
                prev = current;
                current = current->next;
            }
        }
        last_sorted = current; // The last swapped node is now sorted
    } while (swapped);
}
