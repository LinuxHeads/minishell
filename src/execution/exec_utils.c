/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 01:44:02 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/17 05:38:56 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

void	close_fds(t_shell *shell)
{
	if (shell->in_fd != STDIN_FILENO)
		close(shell->in_fd);
	if (shell->out_fd != STDOUT_FILENO)
		close(shell->out_fd);
}
