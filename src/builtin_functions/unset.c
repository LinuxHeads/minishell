/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:42:37 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/11 06:06:37 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_unset(char **args, t_env **envp)
{
	t_env	*tmp;
	t_env	*prev;
	int		i;

	i = -1;
	while (args[++i])
	{
		tmp = *envp;
		prev = NULL;
		while (tmp)
		{
			if (ft_strncmp(args[i], "-", 1) == 0)
			{
				ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
				ft_putstr_fd(args[i], STDERR_FILENO);
				ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
				return (2);
			}
			if (ft_strcmp(args[i], tmp->name) == 0)
			{
				if (prev)
					prev->next = tmp->next;
				else
					*envp = tmp->next;
				free_envp_node(tmp);
				break ;
			}
			prev = tmp;
			tmp = tmp->next;
		}
	}
	return (0);
}
