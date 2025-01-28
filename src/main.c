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
	int i = 0;
	if (envp == NULL)
	{
		return;
	}

	while (envp[i] != NULL)
	{
		if (envp[i] != NULL)
		{
			printf("%s\n", envp[i]);
		}
		i++;
	}
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	print_envp(envp);
	return (0);
}
