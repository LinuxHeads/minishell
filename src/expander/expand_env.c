/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 17:52:57 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/16 07:38:06 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*expand_name(const char *str, int start, int *index, t_shell *shell)
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

static int	expand_digit(char **expanded, t_shell *shell, int *index,
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

char	*expand_env_variable(const char *str, int *index, t_shell *shell)
{
	int		start;
	char	*expanded;
	int		res;

	if (str[*index] == '?')
	{
		(*index)++;
		expanded = ft_itoa(shell->exit_status);
		if (!expanded)
			return (NULL);
		return (expanded);
	}
	start = *index;
	res = expand_digit(&expanded, shell, index, str);
	if (res == 1)
		return (expanded);
	if (res == 2)
		return (NULL);
	else
	{
		while (str[*index] && (ft_isalnum(str[*index]) || str[*index] == '_'))
			(*index)++;
	}
	expanded = expand_name(str, start, index, shell);
	return (expanded);
}

static int	helper_for_some_shit(const char *str, char **result, int i)
{
	int		encl;
	char	*tmp;
	char	*temp;
	char	c[2];

	c[0] = str[i];
	c[1] = '\0';
	encl = count_surrounding_quotes(*result);
	if (encl == 2)
	{
		temp = strip_outers_quotes(*result);
		free(*result);
		*result = temp;
	}
	tmp = ft_strjoin(*result, c);
	if (!tmp)
	{
		free(*result);
		return (0);
	}
	free(*result);
	*result = tmp;
	return (1);
}

static int	loop_dq(char **var_value, char *result)
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

char	*expand_env_string(const char *str, t_shell *shell)
{
	char	*result;
	char	*var_value;
	int		i;
	int		in_sq;
	int		in_dq;
	char	*tmp;
	int		encl;
	char	*temp;
	int		res;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	in_sq = 0;
	in_dq = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_dq)
			in_sq = !in_sq;
		else if (str[i] == '"' && !in_sq)
			in_dq = !in_dq;
		else if (str[i] == '$' && !in_sq)
		{
			i++;
			var_value = expand_env_variable(str, &i, shell);
			if (var_value)
			{
				encl = count_surrounding_quotes(var_value);
				if (encl > 1)
				{
					temp = strip_outers_quotes(var_value);
					free(var_value);
					var_value = temp;
				}
				if (!in_dq)
				{
					res = loop_dq(&var_value, result);
					if (res == 1)
						continue ;
					if (res == 0)
					{
						free(result);
						return (NULL);
					}
				}
				tmp = ft_strjoin(result, var_value);
				free(var_value);
				if (!tmp)
				{
					free(result);
					return (NULL);
				}
				free(result);
				result = tmp;
			}
			continue ;
		}
		else if (!helper_for_some_shit(str, &result, i))
		{
			free(var_value);
			return (NULL);
		}
		i++;
	}
	return (result);
}
