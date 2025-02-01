sdh/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 20:18:07 by abdsalah          #+#    #+#             */
/*   Updated: 2025/01/30 14:31:58 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char *ft_getenv(const char *name, t_env *env)
{
    while (env)
    {
        if (ft_strcmp(env->name, name) == 0)
            return (env->value);
        env = env->next;
    }
    return (NULL);
}

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

t_env *init_envp(char **envp)
{
    int i;
    t_env *head = NULL;
    t_env *tail = NULL;
    t_env *new;
    char *equal_sign;
    int name_len;
    
    i = -1;
    while (envp[++i])
    {
        new = malloc(sizeof(t_env));
        if (!new)
            return (NULL);
        equal_sign = ft_strchr(envp[i], '=');
        name_len = equal_sign - envp[i];
        new->name = ft_substr(envp[i], 0, name_len);//check
        if (!new->name)
        {
            free(new);
            return (NULL);
        }
        new->value = ft_strdup(equal_sign + 1);//check
        if (!new->value)
        {
            free(new->name);
            free(new);
            return (NULL);
        }
        new->next = NULL;
        if (!head)
            head = new;
        else
            tail->next = new;
        tail = new;
    }
    return head;
}


char **envp_to_str(t_env *env)
{
    char **envp;
    int i;
    char *temp;

    if (!env)
        return (NULL);

    envp = malloc(sizeof(char *) * (env_length(env) + 1));
    if (!envp)
        return (NULL);

    i = 0;
    while (env)
    {
        temp = ft_strjoin(env->name, "=");
        envp[i] = ft_strjoin(temp, env->value);
        free(temp);
        i++;
        env = env->next;
    }
    envp[i] = NULL;
    return (envp);
}

void print_envp(t_env *env)
{
  while (env)
  {
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

void ft_setenv(const char *name, const char *value, t_env **env_list)
{
    t_env *env = *env_list;
    t_env *new;

    // Check if the variable already exists
    while (env)
    {
        if (ft_strcmp(env->name, name) == 0)
        {
            free(env->value);
            env->value = ft_strdup(value);
            return;
        }
        env = env->next;
    }
    // If not found, create a new entry
    new = malloc(sizeof(t_env));
    if (!new)
        return;
    new->name = ft_strdup(name);
    new->value = ft_strdup(value);
    new->next = *env_list;
    *env_list = new;
}
