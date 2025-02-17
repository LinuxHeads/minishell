/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 20:19:38 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/17 16:19:49 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_shell(t_exec *shell)
{
	int	i;
	int	j;

	if (!shell)
		return ;
	i = 0;
	while (i < shell->command_count)
	{
		j = 0;
		while (j < shell->commands[i]->token_count)
		{
			free(shell->commands[i]->tokens[j]->value);
			free(shell->commands[i]->tokens[j]);
			j++;
		}
		free(shell->commands[i]->tokens);
		free(shell->commands[i]);
		i++;
	}
	free(shell->commands);
	free(shell);
}

static void	print_error_message(char **message, int fd)
{
	int	i;

	i = 0;
	while (message[i])
	{
		ft_putstr_fd(message[i], fd);
		i++;
	}
}

void	ft_exit_handler(t_shell *shell, void *ptr, char **message,
		int exit_code)
{
	if (message)
		print_error_message(message, STDERR_FILENO);
	if (shell)
	{
		free_envp_list(shell->env_list);
		free_str_array(shell->argv);
		free_envp_array(shell->envp);
		free_shell(shell->parser);
		if (shell->in_fd != STDIN_FILENO)
			close(shell->in_fd);
		if (shell->out_fd != STDOUT_FILENO)
			close(shell->out_fd);
		if (shell->prev_fd != -1)
			close(shell->prev_fd);
		if (shell->pipe_created)
		{
			close(shell->pipe_fd[0]);
			close(shell->pipe_fd[1]);
		}
	}
	if (ptr)
		free(ptr);
	exit(exit_code);
}
