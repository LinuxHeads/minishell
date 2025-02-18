/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 13:31:36 by ahramada          #+#    #+#             */
/*   Updated: 2025/02/18 16:00:15 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*process_var_value_quotes(char *var_value)
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

char	*process_var_value_spaces(char *var_value, int in_dq, char **result)
{
	char	*trim;

	if (!in_dq)
	{
		trim = trim_spaces(var_value);
		free(var_value);
		if (!trim)
		{
			free(*result);
			return (NULL);
		}
		if (is_whitespace(trim))
		{
			free(trim);
			return (ft_strdup(""));
		}
		var_value = compress_whitespace(trim);
		free(trim);
		if (!var_value)
		{
			free(*result);
			return (NULL);
		}
	}
	return (var_value);
}

int if_it_has_space(const char *str) {
    if (!str) return 0; 

    while (*str) {
        if (*str == ' ')
            return 1;
        str++;
    }
    return 0;
}


int	setup_var_value(char **var_value, t_expander *ctx)
{
	*var_value = process_var_value_quotes(*var_value);
	if (!*var_value)
	{
		free(ctx->result);
		return (-1);
	}
	*var_value = process_var_value_spaces(*var_value, ctx->in_dq, &ctx->result);
	if (if_it_has_space(*var_value))
	{
    	*var_value = ft_strjoin(*var_value, " ");
	}
	if (!*var_value)
		return (-1);
	if (!*var_value[0])
	{
		free(*var_value);
		return (0);
	}
	return (1);
}

int	handle_dollar(t_expander *ctx)
{
	char	*var_value;
	char	*tmp;
	int		ret;

	ctx->i++;
	var_value = expand_env_variable(ctx->str, &ctx->i, ctx->shell);
	if (var_value)
	{
		ret = setup_var_value(&var_value, ctx);
		if (ret == -1)
			return (-1);
		if (ret == 0)
			return (0);
		tmp = ft_strjoin(ctx->result, var_value);
		free(var_value);
		if (!tmp)
		{
			free(ctx->result);
			return (-1);
		}
		free(ctx->result);
		ctx->result = tmp;
	}
	return (0);
}

int	append_char(t_expander *ctx)
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
	{
		free(ctx->result);
		return (-1);
	}
	free(ctx->result);
	ctx->result = tmp;
	return (0);
}
