/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_to_str.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 20:18:07 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/17 13:13:08 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	fill_envp(char **envp, t_env *envp_list, int *i)
{
	char	*temp;

	while (envp_list)
	{
		if (envp_list->value != NULL)
		{
			temp = ft_strjoin(envp_list->name, "=");
			if (!temp)
			{
				free_envp_array(envp);
				return (0);
			}
			envp[*i] = ft_strjoin(temp, envp_list->value);
			free(temp);
			if (!envp[*i])
			{
				free_envp_array(envp);
				return (0);
			}
			(*i)++;
		}
		envp_list = envp_list->next;
	}
	envp[*i] = NULL;
	return (1);
}

char	**envp_to_str(t_env *env)
{
	char	**envp;
	int		i;

	if (!env)
		return (NULL);
	envp = malloc(sizeof(char *) * (env_length(env) + 1));
	if (!envp)
		return (NULL);
	i = 0;
	if (!fill_envp(envp, env, &i))
	{
		free_envp_array(envp);
		return (NULL);
	}
	return (envp);
}
