/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 20:18:07 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/01 23:21:07 by abdsalah         ###   ########.fr       */
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
        equal_sign = ft_strchr(envp[i], '=');
        if (!equal_sign) // Skip entries without '='
            continue;    // (e.g., "VAR" instead of "VAR=value")

        new = malloc(sizeof(t_env));
        if (!new)
            return (NULL);

        name_len = equal_sign - envp[i];
        new->name = ft_substr(envp[i], 0, name_len);
        if (!new->name)
        {
            free(new);
            return (NULL);
        }

        // Handle empty values (e.g., "VAR=")
        new->value = ft_strdup(equal_sign + 1); // equal_sign + 1 could be ""
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
        // Skip variables without a value (no '=')
        if (env->value != NULL)
        {
            temp = ft_strjoin(env->name, "=");
            if (!temp)
            {
                free_envp_array(envp);
                return (NULL);
            }
            envp[i] = ft_strjoin(temp, env->value);
            free(temp);
            if (!envp[i])
            {
                free_envp_array(envp);
                return (NULL);
            }
            i++;
        }
        env = env->next;
    }
    envp[i] = NULL;
    return (envp);
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
            // If value is NULL, set env->value to NULL (no '=')
            env->value = value ? ft_strdup(value) : NULL;
            return;
        }
        env = env->next;
    }

    // Create a new entry
    new = malloc(sizeof(t_env));
    if (!new)
        return;
    new->name = ft_strdup(name);
    if (!new->name)
    {
        free(new);
        return;
    }
    // If value is NULL, the variable has no '=' (e.g., `export VAR`)
    new->value = value ? ft_strdup(value) : NULL;
    new->next = NULL;

    // Append to the end of the list to preserve order
    if (!*env_list)
        *env_list = new;
    else
    {
        t_env *last = *env_list;
        while (last->next)
            last = last->next;
        last->next = new;
    }
}
