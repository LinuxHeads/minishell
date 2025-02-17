/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@std.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 20:22:52 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/17 08:41:58 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int			g_signal_flag;

void	process_commands_utils(char **input, char **proc, char ***cmds,
		int *ncmd)
{
	if (**input)
		add_history(*input);
	*proc = preprocess_input(*input);
	*cmds = ft_splitter(*proc, '|');
	*ncmd = count_words(*proc, '|');
}

static void	process_commands(t_shell *shell, char *input)
{
	char	*proc;
	char	**cmds;
	int		ncmd;

	process_commands_utils(&input, &proc, &cmds, &ncmd);
	shell->parser = allocate_shell_commands(ncmd, cmds);
	if (!shell->parser)
	{
		free(proc);
		return ;
	}
	if (!syntax_checker(shell->parser))
	{
		shell->exit_status = 2;
		free_shell(shell->parser);
		free_str_array(cmds);
		free(proc);
		return ;
	}
	execute_pipeline(&shell);
	signals_setup(0);
	free_shell(shell->parser);
	free_str_array(cmds);
	free(proc);
}

void	minishell_loop(t_shell *shell)
{
	char	*input;

	while (1)
	{
		if (g_signal_flag == SIGINT)
			shell->exit_status = 130;
		g_signal_flag = 0;
		input = readline("\001\033[32m\002ZOMBI>\001\033[33m\002 ");
		if (g_signal_flag == SIGINT)
		{
			dup2(2, 0);
			shell->exit_status = 130;
			if (input)
				free(input);
			continue ;
		}
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		process_commands(shell, input);
		free(input);
	}
	rl_clear_history();
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	g_signal_flag = 0;
	init_minishell(&shell, envp);
	signals_setup(0);
	minishell_loop(&shell);
	free_envp_list(shell.env_list);
	free_envp_array(shell.envp);
	return (shell.exit_status);
}
