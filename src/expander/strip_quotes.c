/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strip_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 07:41:56 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/16 07:44:21 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	process_quote_segment(size_t *i, size_t *j, char *result,
		const char *s)
{
	int		found_closing;
	char	quote;
	size_t	len;

	len = strlen(s);
	if (s[*i] == '\'' || s[*i] == '"')
	{
		quote = s[*i];
		found_closing = has_closing(*i, quote, s, len);
		if (found_closing)
		{
			(*i)++;
			while (*i < len && s[*i] != quote)
				result[(*j)++] = s[(*i)++];
			if (*i < len && s[*i] == quote)
				(*i)++;
		}
		else
			result[(*j)++] = s[(*i)++];
		return (1);
	}
	return (0);
}

char	*strip_outers_quotes(const char *s)
{
	size_t	len;
	size_t	i;
	size_t	j;
	char	*result;

	if (!s)
		return (NULL);
	len = strlen(s);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (i < len)
	{
		if (process_quote_segment(&i, &j, result, s))
			continue ;
		else
			result[j++] = s[i++];
	}
	result[j] = '\0';
	return (result);
}
