/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@std.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 19:23:10 by abdsalah          #+#    #+#             */
/*   Updated: 2025/01/26 20:00:48 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void print_envp(char **envp)
{
	if (envp == NULL)
	{
		// If envp is NULL, return early
		return;
	}

	// Loop through the envp array and print each environment variable
	for (int i = 0; envp[i] != NULL; i++)
	{
		// Ensure each element in envp is not NULL
		if (envp[i] != NULL)
		{
			printf("%s\n", envp[i]);  // Print the environment variable
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	print_envp(envp);
	return (0);
}
