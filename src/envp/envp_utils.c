/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 23:20:22 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/04 03:41:32 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int env_length(t_env *env)
{
  int i;

  i = 0;
  while (env)
  {
    i++;
    env = env->next;
  }
  return (i);
}

void print_envp(t_env *env)
{
    while (env)
    {
        // Only print variables with a value (even empty)
        if (env->value != NULL)
            printf("%s=%s\n", env->name, env->value);
        env = env->next;
    }
}

void printstr_envp(char **envp)
{
	int i = 0;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
}
void free_envp_node(t_env *env)
{
	free(env->name);
	free(env->value);
	free(env);
}

void free_envp_list(t_env *env)
{
	t_env *tmp;
	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->name);
		free(tmp->value);
		free(tmp);
	}
}

void free_envp_array(char **envp)
{
	int i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}
