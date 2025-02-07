/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 01:44:02 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/08 01:48:09 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_heredoc(int pipe_fds[2], t_command *cmd, int i)
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
		write(pipe_fds[1], line, ft_strlen(line));
		write(pipe_fds[1], "\n", 1);
		free(line);
	}
}

char	*trim_quotes(char *value)
{
	char	*trimmed;
	size_t	len;

	if (!value)
		return (NULL);
	len = ft_strlen(value);
	trimmed = value;
	if (len >= 2 && ((value[0] == '\"' && value[len - 1] == '\"')
			|| (value[0] == '\'' && value[len - 1] == '\'')))
	{
		trimmed = ft_substr(value, 1, len - 2);
		free(value);
	}
	return (trimmed);
}
