/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 20:22:52 by abdsalah          #+#    #+#             */
/*   Updated: 2025/01/29 23:25:09 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
