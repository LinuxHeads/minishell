/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 17:52:57 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/17 05:52:07 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

static int	append_variable_value(char **result, char *var_value, int in_dq)
{
	char	*tmp;
	int		res;

	if (!in_dq)
	{
		res = normalize_env_value(&var_value, *result);
		if (res == 1)
			return (1);
		if (res == 0)
		{
			free(*result);
			return (0);
		}
	}
	tmp = ft_strjoin(*result, var_value);
	free(var_value);
	if (!tmp)
	{
		free(*result);
		return (0);
	}
	free(*result);
	*result = tmp;
	return (1);
}

static int	process_regular_char(const char *str, char **result, int i)
{
	char	*tmp;
	char	c[2];

	c[0] = str[i];
	c[1] = '\0';
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

static int	handle_env_var(t_exp *exp, t_shell *shell)
{
	char	*var_value;
	int		encl;
	char	*temp;

	var_value = expand_env_variable(exp->str, &exp->i, shell);
	if (!var_value)
		return (1);
	encl = count_surrounding_quotes(var_value);
	if (encl > 1)
	{
		temp = strip_outers_quotes(var_value);
		free(var_value);
		var_value = temp;
	}
	if (!append_variable_value(exp->result, var_value, exp->in_dq))
		return (0);
	while (exp->str[exp->i] && (ft_isalnum(exp->str[exp->i])
			|| exp->str[exp->i] == '_'))
		exp->i++;
	return (1);
}

char	*expand_env_string(const char *str, t_shell *shell)
{
	char	*result;
	t_exp	exp;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	exp = init_exp(str, &result);
	while (exp.str[exp.i])
	{
		if (exp.str[exp.i] == '\'' || exp.str[exp.i] == '"')
			toggle_quotes(exp.str[exp.i], &exp.in_sq, &exp.in_dq);
		else if (exp.str[exp.i] == '$' && !exp.in_sq)
		{
			exp.i++;
			if (!handle_env_var(&exp, shell))
				return (NULL);
			continue ;
		}
		else if (!process_regular_char(exp.str, exp.result, exp.i))
			return (NULL);
		exp.i++;
	}
	return (*exp.result);
}
