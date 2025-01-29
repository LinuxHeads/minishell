/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 20:18:07 by abdsalah          #+#    #+#             */
/*   Updated: 2025/01/29 23:28:11 by abdsalah         ###   ########.fr       */
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

t_env *init_envp(char **envp)
{
    int i;
    t_env *head = NULL;
    t_env *tail = NULL;
    t_env *new;
    char *equal_sign;
    
    i = 0;
    while (envp[i])
    {
        new = malloc(sizeof(t_env));
        if (!new)
            return (NULL);
        
        equal_sign = ft_strchr(envp[i], '=');
        if (!equal_sign) // If there's no '=', skip this entry
        {
            free(new);
            i++;
            continue;
        }
        int name_len = equal_sign - envp[i];
        new->name = ft_substr(envp[i], 0, name_len);//check
        new->value = ft_strdup(equal_sign + 1);//check
        new->next = NULL;

        if (!head)
            head = new;
        else
            tail->next = new;
        
        tail = new;
        i++;
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
// void ft_setenv(const char *name, const char *value, char **envp)
// {
//   char *p;

//   if (getenv(name) == NULL)
//   {

//   }
// }