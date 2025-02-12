/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 20:22:52 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/12 23:10:35 by ahramada         ###   ########.fr       */
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
	shell->env_list = init_envp(envp);
    if (!shell->env_list)
	{
        printf("Error: env list is NULL\n");
        exit(1);
    }
	ft_setup_shlvl(&shell->env_list);
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

char *get_multiline_input(void)
{
    char    *input;
    char    *line;
    char    *temp;
    int     in_quotes;

    input = readline("minishell$ ");
    if (!input) // Handle Ctrl+D
        return (NULL);

    in_quotes = 0;
    line = input;
    while (*line)
    {
        if (*line == '"' || *line == '\'')
            in_quotes = !in_quotes;
        line++;
    }
    while (in_quotes) // Keep reading if quotes are open
    {
        temp = readline("> ");
        if (!temp) // Handle Ctrl+D in multi-line input
        {
            free(input);
            return (NULL);
        }
        input = ft_strjoin(input, "\n");  // Append newline
        input = ft_strjoin(input, temp);  // Append next line
        free(temp);
        in_quotes = 0;
        line = input;
        while (*line)
        {
            if (*line == '"' || *line == '\'')
                in_quotes = !in_quotes;
            line++;
        }
    }
    return (input);
}

void minishell_loop(t_shell *shell)
{
	char	*input;
	char	*processed_input;
	char	**commands;
	int		num_commands;

	// rl_bind_key('\t', rl_insert);// this fixes all edge cases
	while (1)
	{
		if (g_signal_flag == SIGINT)
		{
			shell->exit_status = 130;
		}
		g_signal_flag = 0;
		// input = get_multiline_input(); //handles none closed qoutes
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
		processed_input = preprocess_input(input);
		commands = ft_splitter(processed_input, '|');
		num_commands = count_words(processed_input, '|');
		shell->parser = allocate_shell_commands(num_commands, commands); 
		if (!shell->parser)
		{
			free(processed_input);
			free(input);
			continue;
		}
		// print_shell(shell->parser); //if we need to print the commands 
		if (!syntax_checker(shell->parser))
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
