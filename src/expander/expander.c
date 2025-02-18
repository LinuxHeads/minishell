/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 07:38:40 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/18 05:03:40 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Allocates a new token that keeps only the first two characters.
*/
static char	*handle_quote_token(char *token)
{
	char	*c;

	c = malloc(3 * sizeof(char));
	if (!c)
		return (NULL);
	c[0] = token[0];
	c[1] = token[1];
	c[2] = '\0';
	return (c);
}

static int	tilde_expander_helper(t_shell *shell, char **token_ptr,
		char **old_arg)
{
	char	*expanded;
	char	*home;

	home = ft_getenv("HOME", shell->env_list);
	if (!home)
		home = "";
	if ((*old_arg)[1] == '\0')
		expanded = ft_strdup(home);
	else
		expanded = ft_strjoin(home, *old_arg + 1);
	if (!expanded)
		return (0);
	free(*token_ptr);
	*token_ptr = expanded;
	*old_arg = expanded;
	return (1);
}

static int	process_no_env_token(char **token_ptr, t_shell *shell)
{
	char	*old_arg;
	char	*no_closed;
	char	*tmp;

	old_arg = *token_ptr;
	if (old_arg[0] == '~' && (old_arg[1] == '/' || old_arg[1] == '\0'))
	{
		if (!tilde_expander_helper(shell, token_ptr, &old_arg))
			return (-1);
	}
	no_closed = strip_outers_quotes(old_arg);
	if (no_closed)
	{
		free(*token_ptr);
		*token_ptr = no_closed;
	}
	tmp = cleanup_input(*token_ptr);
	if (tmp)
	{
		free(*token_ptr);
		*token_ptr = tmp;
	}
	return (0);
}

/*
** The expander function iterates over the argv tokens and processes each:
**
** - If the token is a simple quote token, it is reduced to its first two
**   characters.
** - If it contains no '$', it is cleaned-up (with "~" expanded to HOME).
** - Otherwise, environment variables are expanded, cleaned and if the
**   resulting token contains spaces, it may be split into multiple tokens.
**
** The function returns the number of tokens in the resulting argv.
*/
int	process_simple_token(char ***argv, int i, t_shell **shell)
{
	int		ret;
	char	*tmp;

	if (is_single_quote_token((*argv)[i]) || is_double_quote_token((*argv)[i]))
	{
		ret = 0;
		tmp = handle_quote_token((*argv)[i]);
		if (!tmp)
			return (-1);
		free((*argv)[i]);
		(*argv)[i] = tmp;
		return (1);
	}
	if (!ft_strchr((*argv)[i], '$'))
	{
		ret = process_no_env_token(&((*argv)[i]), *shell);
		if (ret < 0)
			return (-1);
		return (1);
	}
	return (0);
}

int	expander(t_shell **shell)
{
	char	**argv;
	int		i;
	int		ret;

	argv = (*shell)->argv;
	i = 0;
	while (argv[i])
	{
		ret = process_simple_token(&argv, i, shell);
		if (ret <= 0)
			if (ret < 0)
				return (-1);
		if (ret == 1)
			i++;
		if (ret == 1)
			continue ;
		ret = process_env_token(&argv, *shell, i);
		if (ret < 0)
			return (-1);
		if (ret == 1)
			i++;
	}
	(*shell)->argv = argv;
	return (i);
}
