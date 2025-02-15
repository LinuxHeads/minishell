/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_qoutes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 17:48:42 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/15 17:51:33 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_surrounding_quotes(const char *str)
{
	int			count;
	int			len;
	const char	*start;
	const char	*end;

	count = 0;
	len = ft_strlen(str);
	start = str;
	end = str + len - 1;
	while (len >= 2 &&
			((*start == '\'' && *end == '\'') || (*start == '"' && *end == '"')))
	{
		count++;
		start++;
		end--;
		len -= 2;
	}
	return (count);
}

int	is_single_quote_token(char *s)
{
	int	i;

	i = 0;
	while (s[i] == '\'' && s[i])
	{
		i++;
		if (s[i] != '\'' && s[i])
			return (0);
	}
	return (i);
}

int	is_double_quote_token(char *s)
{
	int	i;

	i = 0;
	while (s[i] == '\"' && s[i])
	{
		i++;
		if (s[i] != '\"' && s[i])
			return (0);
	}
	return (i);
}
