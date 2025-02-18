/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_cleanup_tokens.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:56:47 by ahramada          #+#    #+#             */
/*   Updated: 2025/02/18 17:57:00 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*helper(char **argv, int i, char *expanded)
{
	char	*tmp;

	if (argv[i][0] == '$' && argv[i][1] != '\0')
		tmp = expanded;
	else if (argv[i][0] == '\"' && argv[i][1] == '$' && argv[i][2] != '\0')
		tmp = expanded;
	else if (argv[i][0] == '\'' && argv[i][1] == '$' && argv[i][2] != '\0')
		tmp = expanded;
	else
	{
		tmp = cleanup_input(expanded);
		free(expanded);
	}
	return (tmp);
}

char	*expand_and_cleanup_token(char ***argv_ptr, char **argv, int i,
		t_shell *shell)
{
	char	*tmp;
	char	*expanded;

	expanded = expand_env_string(argv[i], shell);
	if (!expanded || is_whitespace(expanded))
	{
		free(expanded);
		remove_argument(argv_ptr, i);
		return (0);
	}
	tmp = helper(argv, i, expanded);
	if (!tmp)
	{
		remove_argument(argv_ptr, i);
		return (0);
	}
	expanded = tmp;
	return (expanded);
}
