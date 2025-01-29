/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@std.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 20:22:52 by abdsalah          #+#    #+#             */
/*   Updated: 2025/01/29 20:25:04 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
void init_minishell(char **envp)
{
    t_env *env = init_envp(envp);
    
    if (!env) {
        printf("Error: env list is NULL\n");
        return;
    }

    char **env_str = envp_to_str(env);

    if (!env_str || !env_str[0]) {
        printf("Error: envp_to_str returned NULL or empty\n");
        return;
    }

    printstr_envp(env_str);

    // Free memory
    free_envp_list(env);
    free_envp_array(env_str);
}


int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	init_minishell(envp);
	return (0);
}
