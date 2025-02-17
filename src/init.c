/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 20:22:52 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/17 06:06:37 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	env_exit(t_shell *shell, char **envp)
{
	if (!envp)
	{
		printf("Error: envp is NULL\n");
		shell->env_list = NULL;
		shell->envp = NULL;
		shell->exit_status = 0;
		shell->parser = NULL;
		return (0);
	}
	return (1);
}

static void	env_init(t_shell *shell, char **envp)
{
	if (!env_exit(shell, envp))
	{
		return ;
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
}

void	init_minishell(t_shell *shell, char **envp)
{
	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO)
		|| !isatty(STDERR_FILENO))
	{
		fprintf(stderr, "Error: Minishell must be run in a terminal (TTY)\n");
		exit(1);
	}
	env_init(shell, envp);
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
