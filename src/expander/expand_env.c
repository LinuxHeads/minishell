/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 12:39:10 by ahramada          #+#    #+#             */
/*   Updated: 2025/02/17 12:47:25 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

typedef struct s_expander
{
	const char	*str;
	int			i;
	int			in_sq;
	int			in_dq;
	t_shell		*shell;
	char		*result;
}				t_expander;

static char	*expand_env_variable_non_digit(const char *str, int *index,
		t_shell *shell, int start)
{
	char	*var_name;
	char	*env_val;
	char	*expanded;

	while (str[*index] && (ft_isalnum(str[*index]) || str[*index] == '_'))
		(*index)++;
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

char	*expand_env_variable(const char *str, int *index, t_shell *shell)
{
	int		start;
	char	*var_name;
	char	*env_val;
	char	*expanded;

	if (str[*index] == '?')
	{
		(*index)++;
		return (ft_itoa(shell->exit_status));
	}
	start = *index;
	if (ft_isdigit(str[start]))
	{
		var_name = ft_substr(str, start, *index - start);
		if (!var_name)
			return (NULL);
		env_val = ft_getenv(var_name, shell->env_list);
		free(var_name);
		if (!env_val)
			env_val = "";
		expanded = ft_strdup(env_val);
		(*index)++;
		return (expanded);
	}
	return (expand_env_variable_non_digit(str, index, shell, start));
}

static char	*process_var_value_quotes(char *var_value)
{
	char	*temp;

	if (count_surrounding_quotes(var_value) > 1)
	{
		temp = strip_outers_quotes(var_value);
		free(var_value);
		return (temp);
	}
	return (var_value);
}

static char	*process_var_value_spaces(char *var_value, int in_dq, char **result)
{
	char	*trim;

	if (!in_dq)
	{
		trim = trim_spaces(var_value);
		free(var_value);
		if (!trim)
			return (free(*result), (char *)0);
		if (is_whitespace(trim))
		{
			free(trim);
			return (ft_strdup(""));
		}
		var_value = compress_whitespace(trim);
		free(trim);
		if (!var_value)
			return (free(*result), (char *)0);
	}
	return (var_value);
}

static int	handle_dollar(t_expander *ctx)
{
	char	*var_value;
	char	*tmp;

	ctx->i++;
	var_value = expand_env_variable(ctx->str, &ctx->i, ctx->shell);
	if (var_value)
	{
		var_value = process_var_value_quotes(var_value);
		if (!var_value)
			return (free(ctx->result), -1);
		var_value = process_var_value_spaces(var_value, ctx->in_dq,
				&ctx->result);
		if (!var_value)
			return (-1);
		if (!*var_value)
			return (free(var_value), 0);
		tmp = ft_strjoin(ctx->result, var_value);
		free(var_value);
		if (!tmp)
			return (free(ctx->result), -1);
		free(ctx->result);
		ctx->result = tmp;
	}
	return (0);
}

static int	append_char(t_expander *ctx)
{
	char	c[2];
	char	*tmp;
	int		encl;
	char	*temp;

	c[0] = ctx->str[ctx->i];
	c[1] = '\0';
	encl = count_surrounding_quotes(ctx->result);
	if (encl == 2)
	{
		temp = strip_outers_quotes(ctx->result);
		free(ctx->result);
		ctx->result = temp;
	}
	tmp = ft_strjoin(ctx->result, c);
	if (!tmp)
		return (free(ctx->result), -1);
	free(ctx->result);
	ctx->result = tmp;
	return (0);
}

static int	process_char(t_expander *ctx)
{
	if (ctx->str[ctx->i] == '\'' && !ctx->in_dq)
		ctx->in_sq = !ctx->in_sq;
	else if (ctx->str[ctx->i] == '"' && !ctx->in_sq)
		ctx->in_dq = !ctx->in_dq;
	else if (ctx->str[ctx->i] == '$' && !ctx->in_sq)
	{
		if (handle_dollar(ctx) == -1)
			return (-1);
		return (1);
	}
	else
	{
		if (append_char(ctx) == -1)
			return (-1);
	}
	return (0);
}

char	*expand_env_string(const char *str, t_shell *shell)
{
	t_expander	ctx;
	int			ret;

	ctx.str = str;
	ctx.i = 0;
	ctx.in_sq = 0;
	ctx.in_dq = 0;
	ctx.shell = shell;
	ctx.result = ft_strdup("");
	if (!ctx.result)
		return (NULL);
	while (ctx.str[ctx.i])
	{
		ret = process_char(&ctx);
		if (ret == -1)
			return (NULL);
		if (!ret)
			ctx.i++;
	}
	return (ctx.result);
}
