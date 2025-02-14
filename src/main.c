/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 20:22:52 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/14 14:45:54 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_signal_flag;

void init_minishell(t_shell *shell, char **envp)//$SHLVL
{
	// if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO) || !isatty(STDERR_FILENO))
    // {
    //     fprintf(stderr, "Error: Minishell must be run in a terminal (TTY)\n");
    //     exit(1);
    // }
	if (!envp)
	{
		printf("Error: envp is NULL\n");
		shell->env_list = NULL;
		shell->envp = NULL;
		shell->exit_status = 0;
		shell->parser = NULL;
		return;
	}
	shell->env_list = init_envp(envp);
    if (!shell->env_list)
	{
        printf("Error: env list is NULL\n");
        exit(1);
    }
	if (!ft_setup_shlvl(&shell->env_list))
	{
		printf("Error: ft_setup_shlvl returned 0\n");
		free_envp_list(shell->env_list);
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
	shell->argv = NULL;
	shell->input = NULL;
	shell->in_fd = -1;
	shell->out_fd = -1;
	shell->prev_fd = -1;
	shell->pipe_created = 0;
	shell->pipe_fd[0] = -1;
	shell->pipe_fd[1] = -1;
}

void minishell_loop(t_shell *shell)
{
	char	*input;
	char	*processed_input;
	char	**commands;
	int		num_commands;

	while (1)
	{
		if (g_signal_flag == SIGINT)
		{
			shell->exit_status = 130;
		}
		g_signal_flag = 0;
		input = readline("\001\033[32m\002ZOMBI>\001\033[33m\002 ");
		
		if (g_signal_flag == SIGINT)
        {
			dup2 (2, 0);
            shell->exit_status = 130;
            if (input)
                free(input);
            continue;
        }
		if (!input)
		{
			
			printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		processed_input = preprocess_input(input);// there is a memory leak here that i can't fix
		commands = ft_splitter(processed_input, '|');// no leaks
		num_commands = count_words(processed_input, '|');// no leaks
		shell->parser = allocate_shell_commands(num_commands, commands); // no leaks
		if (!shell->parser)
		{
			free(processed_input); // does not work
			free(input);
			continue;
		}
		// print_shell(shell->parser); //if we need to print the commands 
		if (!syntax_checker(shell->parser)) // no leaks
		{
			shell->exit_status = 2;
			free_shell(shell->parser);
			free_str_array(commands);
			free(processed_input);
			free(input);
			continue;
		}
		execute_pipeline(&shell);
        signals_setup(0);
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
	
	g_signal_flag = 0;
	t_shell shell;
	init_minishell(&shell, envp);
	signals_setup(0);
	minishell_loop(&shell);
	free_envp_list(shell.env_list);
	free_envp_array(shell.envp);
	return (shell.exit_status);
}
