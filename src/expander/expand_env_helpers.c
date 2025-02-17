/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 05:48:40 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/17 05:52:23 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_exp	init_exp(const char *str, char **res)
{
	t_exp	exp;

	exp.str = str;
	exp.i = 0;
	exp.result = res;
	exp.in_sq = 0;
	exp.in_dq = 0;
	return (exp);
}

void	toggle_quotes(char c, int *in_sq, int *in_dq)
{
	if (c == '\'' && !(*in_dq))
		*in_sq = !(*in_sq);
	else if (c == '"' && !(*in_sq))
		*in_dq = !(*in_dq);
}

char	*expand_name(const char *str, int start, int *index, t_shell *shell)
{
	char	*var_name;
	char	*env_val;
	char	*expanded;

	if (start == *index)
		return (ft_strdup("$"));
	var_name = ft_substr(str, start, *index - start);
	if (!var_name)
		return (NULL);
	env_val = ft_getenv(var_name, shell->env_list);
	free(var_name);
	if (!env_val)
		env_val = "";
	expanded = ft_strdup(env_val);
	return (expanded);
}

int	expand_digit(char **expanded, t_shell *shell, int *index,
		const char *str)
{
	char	*var_name;
	char	*env_val;
	int		start;

	start = *index;
	if (ft_isdigit(str[start]))
	{
		var_name = ft_substr(str, start, *index - start);
		if (!var_name)
			return (2);
		env_val = ft_getenv(var_name, shell->env_list);
		free(var_name);
		if (!env_val)
			env_val = "";
		*expanded = ft_strdup(env_val);
		(*index)++;
		return (1);
	}
	return (0);
}

int	normalize_env_value(char **var_value, char *result)
{
	char	*trim;

	trim = trim_spaces(*var_value);
	free(*var_value);
	if (!trim)
	{
		free(result);
		return (0);
	}
	if (is_whitespace(trim))
	{
		free(trim);
		return (1);
	}
	*var_value = compress_whitespace(trim);
	free(trim);
	if (!*var_value)
	{
		free(result);
		return (0);
	}
	return (2);
}
