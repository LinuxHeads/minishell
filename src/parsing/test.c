/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 07:12:11 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/06 07:12:25 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* --- Process one iteration of the shell loop --- */

static int	process_shell_iteration(void)
{
	t_shell	*shell;
	char	**commands;
	int		num_commands;
	char	*input;

	input = readline("\033[1;32mminishell>\033[0m ");
	if (!input)
	{
		printf("\033[1;31mexit\033[0m\n");
		return (0);
	}
	shell = malloc(sizeof(t_shell));
	if (*input)
		add_history(input);
	input = preprocess_input(input);
	commands = ft_splitter(input, '|');
	num_commands = count_words(input, '|');
	shell->parser = allocate_shell_commands(num_commands, commands);
	print_shell(shell->parser);
	free_shell(shell->parser);
	free_str_array(commands);
	free(input);
	return (1);
}

/* --- Loop helper --- */

static void	run_shell_loop(void)
{
	while (process_shell_iteration())
		;
	clear_history();
}

int	main(void)
{
	run_shell_loop();
	return (0);
}
