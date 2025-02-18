/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_setenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 04:50:36 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/18 10:00:35 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* --- Updates existing environment variable --- */
static int	update_existing_env(t_env *env, const char *value)
{
	free(env->value);
	if (value)
	{
		env->value = ft_strdup(value);
		if (!env->value)
			return (0);
	}
	else
		env->value = NULL;
	return (1);
}

/* --- Creates a new environment variable entry --- */
static t_env	*create_new_env_entry(const char *name, const char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->name = ft_strdup(name);
	if (!new->name)
	{
		free(new);
		return (NULL);
	}
	if (value)
	{
		new->value = ft_strdup(value);
		if (!new->value)
		{
			free(new->name);
			free(new);
			return (NULL);
		}
	}
	else
		new->value = NULL;
	new->next = NULL;
	return (new);
}

/* --- Appends new entry to the environment list --- */
static void	append_env_entry(t_env **env_list, t_env *new)
{
	t_env	*last;

	last = *env_list;
	if (!last)
		*env_list = new;
	else
	{
		while (last->next)
			last = last->next;
		last->next = new;
	}
}

/* --- Main function to set environment variable --- */
int	ft_setenv(const char *name, const char *value, t_env **env_list)
{
	t_env	*env;
	t_env	*new;

	if (!env_list)
	{
		new = create_new_env_entry(name, value);
		if (!new)
			return (0);
	}
	env = *env_list;
	while (env)
	{
		if (ft_strcmp(env->name, name) == 0)
		{
			if (update_existing_env(env, value))
				return (1);
			return (0);
		}
		env = env->next;
	}
	new = create_new_env_entry(name, value);
	if (!new)
		return (0);
	append_env_entry(env_list, new);
	return (1);
}
