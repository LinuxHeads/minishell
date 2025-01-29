/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 20:22:52 by abdsalah          #+#    #+#             */
/*   Updated: 2025/01/30 01:49:00 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


void init_minishell(t_shell *shell, char **envp)
{
	shell->env_list = init_envp(envp);
    if (!shell->env_list)
	{
        printf("Error: env list is NULL\n");
        exit(1);
    }
	shell->envp = envp_to_str(shell->env_list);
	if (!shell->envp || !shell->envp[0])
	{
		printf("Error: envp_to_str returned NULL or empty\n");
		exit(1);
	}
	shell->exit_status = 0;
}

void minishell_loop()
{
	char *line;

	while (1)
	{
		line = readline("minishell $>");
		if (!line)
			break;
		if (ft_strlen(line) > 0)
			add_history(line);
		if (ft_strcmp(line, "exit") == 0)
		{
			free(line);
			break;
		}
		free(line);
	}
}
int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	t_shell shell;
	init_minishell(&shell, envp);
	signals_t3res();
	minishell_loop();
	return (shell.exit_status);
}
