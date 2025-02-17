/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 05:37:10 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/17 05:39:44 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	close_heredoc(int signum)
{
	g_signal_flag = signum;
	close(0);
}

static void	heredoc_loop(int pipe_fds[2], t_command *cmd, int i, t_shell *shell)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, cmd->tokens[i]->value) == 0)
		{
			free(line);
			break ;
		}
		expand_single_argument(&line, shell);
		if (line)
		{
			write(pipe_fds[1], line, ft_strlen(line));
			write(pipe_fds[1], "\n", 1);
			free(line);
		}
		else
			write(pipe_fds[1], "\n", 1);
	}
}

void	ft_heredoc(int pipe_fds[2], t_command *cmd, int i, t_shell *shell)
{
	reset_signals_heredoc();
	heredoc_loop(pipe_fds, cmd, i, shell);
	signals_setup(0);
	if (g_signal_flag == SIGINT)
		write(1, "\n", 1);
	if (dup2(2, 0) < 0)
		perror("dup2");
}
