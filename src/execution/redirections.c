/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 00:46:38 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/16 13:14:31 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	ft_setup_infile(t_command *cmd, int *in_fd, int *i, t_shell *shell)
{
	if (cmd->tokens[*i]->type == REDIRECT_IN)
	{
		if (*i + 1 < cmd->token_count && cmd->tokens[*i + 1])
		{
			(*i)++;
			expand_single_argument(&cmd->tokens[*i]->value, shell);
			cmd->tokens[*i]->value = trim_quotes(cmd->tokens[*i]->value);
			if (cmd->tokens[*i]->value == NULL)
				return (0);
			*in_fd = open(cmd->tokens[*i]->value, O_RDONLY);
			if (*in_fd < 0)
			{
				*in_fd = STDIN_FILENO;
				perror(cmd->tokens[*i]->value);
				return (0);
			}
		}
		else
		{
			ft_putstr_fd("minishell: syntax error.\n", 2);
			return (0);
		}
	}
	return (1);
}

static int	ft_setup_outfile(t_command *cmd, int *out_fd, int *i,
		t_shell *shell)
{
	if (cmd->tokens[*i]->type == REDIRECT_OUT)
	{
		if (*i + 1 < cmd->token_count && cmd->tokens[*i + 1])
		{
			(*i)++;
			expand_single_argument(&cmd->tokens[*i]->value, shell);
			cmd->tokens[*i]->value = trim_quotes(cmd->tokens[*i]->value);
			if (cmd->tokens[*i]->value == NULL)
				return (0);
			*out_fd = open(cmd->tokens[*i]->value, O_WRONLY | O_CREAT | O_TRUNC,
					0644);
			if (*out_fd < 0)
			{
				*out_fd = STDOUT_FILENO;
				perror(cmd->tokens[*i]->value);
				return (0);
			}
		}
		else
		{
			ft_putstr_fd("minishell: syntax error.\n", 2);
			return (0);
		}
	}
	return (1);
}

static int	ft_setup_append(t_command *cmd, int *out_fd, int *i, t_shell *shell)
{
	if (cmd->tokens[*i]->type == REDIRECT_APPEND)
	{
		if (*i + 1 < cmd->token_count && cmd->tokens[*i + 1])
		{
			(*i)++;
			expand_single_argument(&cmd->tokens[*i]->value, shell);
			cmd->tokens[*i]->value = trim_quotes(cmd->tokens[*i]->value);
			if (cmd->tokens[*i]->value == NULL)
				return (0);
			*out_fd = open(cmd->tokens[*i]->value,
					O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (*out_fd < 0)
			{
				*out_fd = STDOUT_FILENO;
				perror(cmd->tokens[*i]->value);
				return (0);
			}
		}
		else
		{
			ft_putstr_fd("minishell: syntax error.\n", 2);
			return (0);
		}
	}
	return (1);
}

static int	ft_setup_heredoc(t_command *cmd, int *in_fd, int *i, t_shell *shell)
{
	int	pipe_fds[2];

	if (cmd->tokens[*i]->type == HEREDOC)
	{
		if (*i + 1 < cmd->token_count)
		{
			(*i)++;
			cmd->tokens[*i]->value = trim_quotes(cmd->tokens[*i]->value);
			if (cmd->tokens[*i]->value == NULL)
				return (0);
			if (pipe(pipe_fds) == -1)
			{
				perror("pipe");
				exit(EXIT_FAILURE);
			}
			ft_heredoc(pipe_fds, cmd, *i, shell);
			close(pipe_fds[1]);
			*in_fd = pipe_fds[0];
		}
	}
	return (1);
}

int	get_redirections(t_command *cmd, int *in_fd, int *out_fd, t_shell *shell)
{
	int	i;

	i = 0;
	if (!cmd || !in_fd || !out_fd)
		return (0);
	*in_fd = STDIN_FILENO;
	*out_fd = STDOUT_FILENO;
	while (i < cmd->token_count)
	{
		if (cmd->tokens[i] == NULL)
		{
			i++;
			continue ;
		}
		if (ft_setup_infile(cmd, in_fd, &i, shell) == 0)
			return (0);
		if (ft_setup_outfile(cmd, out_fd, &i, shell) == 0)
			return (0);
		if (ft_setup_append(cmd, out_fd, &i, shell) == 0)
			return (0);
		if (ft_setup_heredoc(cmd, in_fd, &i, shell) == 0)
			return (0);
		i++;
	}
	return (1);
}
