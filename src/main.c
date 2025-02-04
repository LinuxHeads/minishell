/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 20:22:52 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/04 05:05:56 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


void init_minishell(t_shell *shell, char **envp)//$SHLVL
{
	if (!isatty(STDIN_FILENO))
    {
        // If not a TTY, print an error and exit
        fprintf(stderr, "Error: Minishell must be run in a terminal (TTY)\n");
        exit(1);
    }
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
		free_envp_list(shell->env_list);
		exit(1);
	}
	shell->exit_status = 0;
	shell->parser = NULL;
}

void minishell_loop(t_shell *shell)
{
	char	*input;
	char	*processed_input;
	char	**commands;
	int		num_commands;

	while (1)
	{
		input = readline("\033[1;32mminishell>\033[0m ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		processed_input = preprocess_input(input);
		commands = ft_split(processed_input, '|');
		num_commands = count_words(processed_input, '|');
		shell->parser = allocate_shell_commands(num_commands, commands); 
		if (!shell->parser)
		{
			free(processed_input);
			free(input);
			continue;
		}
		execute_pipeline(&shell);
		free_shell(shell->parser);
		free_str_array(commands);
		free(processed_input);
		free(input);
	}
	clear_history();
}

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	t_shell shell;
	init_minishell(&shell, envp);
	signals_t3res();
	minishell_loop(&shell);
	free_envp_list(shell.env_list);
	free_envp_array(shell.envp);
	return (shell.exit_status);
}
