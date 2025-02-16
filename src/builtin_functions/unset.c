/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:42:37 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/16 13:26:17 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	for_the_norme(char *arg)
{
	ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
}

static int	unset_one(char *arg, t_env **envp)
{
	t_env	*tmp;
	t_env	*prev;

	if (ft_strncmp(arg, "-", 1) == 0)
	{
		for_the_norme(arg);
		return (2);
	}
	tmp = *envp;
	prev = NULL;
	while (tmp)
	{
		if (ft_strcmp(arg, tmp->name) == 0)
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
	return (0);
}

int	ft_unset(char **args, t_env **envp)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (args[i])
	{
		if (unset_one(args[i], envp))
			ret = 2;
		i++;
	}
	return (ret);
}
