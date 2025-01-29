/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@std.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 20:18:07 by abdsalah          #+#    #+#             */
/*   Updated: 2025/01/29 20:26:20 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void add_variable(char *envp, t_env *env)
{
    t_env *new;
    char *equal_sign;
    int length;

    equal_sign = ft_strchr(envp, '=');
    if (!equal_sign)
        return; // Invalid environment variable (no '=' found)

    length = equal_sign - envp;
    new = malloc(sizeof(t_env));
    if (!new)
        return;

    new->name = ft_substr(envp, 0, length); // Extract name
    new->value = ft_strdup(equal_sign + 1); // Extract value
    new->next = NULL;

    while (env->next)
        env = env->next;
    env->next = new;
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
    int i = 0;
    t_env *head = NULL;
    t_env *tail = NULL;

    while (envp[i])
    {
        t_env *new = malloc(sizeof(t_env));
        if (!new)
            return (NULL);
        
        char *equal_sign = ft_strchr(envp[i], '=');
        if (!equal_sign) // If there's no '=', skip this entry
        {
            free(new);
            i++;
            continue;
        }
        int name_len = equal_sign - envp[i];
        new->name = ft_substr(envp[i], 0, name_len);
        new->value = ft_strdup(equal_sign + 1);
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
// void ft_setenv(const char *name, const char *value, char **envp)
// {
//   char *p;

//   if (getenv(name) == NULL)
//   {

//   }
// }