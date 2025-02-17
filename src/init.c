/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 20:22:52 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/17 14:22:58 by ahramada         ###   ########.fr       */
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
		ft_exit_handler(shell, 0, (char *[]){"Error: env list is NULL\n", NULL},
			1);
	if (!ft_setup_shlvl(&shell->env_list))
	{
		free_envp_list(shell->env_list);
		ft_exit_handler(shell, 0,
			(char *[]){"Error: ft_setup_shlvl returned 0\n", NULL}, 1);
	}
	shell->envp = envp_to_str(shell->env_list);
	if (!shell->envp || !shell->envp[0])
	{
		free_envp_list(shell->env_list);
		ft_exit_handler(shell, 0,
			(char *[]){"Error: envp_to_str returned NULL or empty\n", NULL}, 1);
	}
}

void	init_minishell(t_shell *shell, char **envp)
{
	// if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO)
	// 	|| !isatty(STDERR_FILENO))
	// {
	// 	fprintf(stderr, "Error: Minishell must be run in a terminal (TTY)\n");
	// 	exit(1);
	// }
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
