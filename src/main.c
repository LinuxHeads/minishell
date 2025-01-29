/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@std.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 19:23:10 by abdsalah          #+#    #+#             */
/*   Updated: 2025/01/29 20:09:46 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


void printstr_envp(char **envp)
{
	int i;

	i = 0;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
}

void init_minishell(char **envp)
{
	
	t_env *env = init_envp(envp);
	printstr_envp(envp_to_str(env));
}


int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	init_minishell(envp);

	return (0);
}
