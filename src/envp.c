/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@std.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 20:18:07 by abdsalah          #+#    #+#             */
/*   Updated: 2025/01/29 20:21:52 by abdsalah         ###   ########.fr       */
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
    int i;
    t_env *env;
    t_env *head;

    if (!envp || !envp[0])
        return (NULL);

    head = malloc(sizeof(t_env));
    if (!head)
        return (NULL);
    head->next = NULL;
    add_variable(envp[0], head);

    env = head;
    i = 1;
    while (envp[i])
    {
        add_variable(envp[i], env);
        i++;
    }

    return (head);
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