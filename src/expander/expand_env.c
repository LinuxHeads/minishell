/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 12:39:10 by ahramada          #+#    #+#             */
/*   Updated: 2025/02/17 13:39:31 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
