/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 23:20:22 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/17 13:08:38 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	env_length(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);
}

void	print_envp(t_env *env)
{
	while (env)
	{
		if (env->value != NULL)
			printf("%s=%s\n", env->name, env->value);
		env = env->next;
	}
}

void	printstr_envp(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
}

static t_env	*init_node(t_env *env, t_env *new_head, t_env *new_node)
{
	new_node->name = ft_strdup(env->name);
	if (!new_node->name)
	{
		free_envp_list(new_head);
		return (NULL);
	}
	if (env->value)
	{
		new_node->value = ft_strdup(env->value);
		if (!new_node->value)
		{
			free_envp_list(new_head);
			return (NULL);
		}
	}
	else
	{
		new_node->value = NULL;
	}
	new_node->next = NULL;
	return (new_node);
}

t_env	*ft_copy_env(t_env *env)
{
	t_env	*new_head;
	t_env	*new_tail;
	t_env	*new_node;

	new_head = NULL;
	new_tail = NULL;
	while (env)
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			return ((t_env *)free_envp_list(new_head));
		if (!init_node(env, new_head, new_node))
		{
			free(new_node);
			return (NULL);
		}
		if (!new_head)
			new_head = new_node;
		else
			new_tail->next = new_node;
		new_tail = new_node;
		env = env->next;
	}
	return (new_head);
}

