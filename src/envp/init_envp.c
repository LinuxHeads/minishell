/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_envp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 05:14:15 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/18 08:43:31 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_env	*add_envp_node(char *env_entry)
{
	t_env	*new;
	int		name_len;
	char	*equal_sign;

	equal_sign = ft_strchr(env_entry, '=');
	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	name_len = equal_sign - env_entry;
	new->name = ft_substr(env_entry, 0, name_len);
	if (!new->name)
	{
		free(new);
		return (NULL);
	}
	new->value = ft_strdup(equal_sign + 1);
	if (!new->value)
	{
		free(new->name);
		free(new);
		return (NULL);
	}
	new->next = NULL;
	return (new);
}

t_env	*init_envp(char **envp)
{
	int		i;
	t_env	*head;
	t_env	*tail;
	t_env	*new;

	if (!envp)
		return (NULL);
	head = NULL;
	tail = NULL;
	i = -1;
	while (envp[++i])
	{
		if (!ft_strchr(envp[i], '='))
			continue ;
		new = add_envp_node(envp[i]);
		if (!new)
		{
			free_envp_list(head);
			return (NULL);
		}
		if (!head)
			head = new;
		else
			tail->next = new;
		tail = new;
	}
	return (head);
}
