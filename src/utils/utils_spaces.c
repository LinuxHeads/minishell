/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_spaces.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 17:50:11 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/15 17:56:09 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_whitespace(const char *str)
{
	int	i;

	if (!str || !*str)
		return (1);
	i = 0;
	while (str[i])
	{
		if (str[i] != ' ' && (str[i] < '\t' || str[i] > '\r'))
			return (0);
		i++;
	}
	return (1);
}

char	*trim_spaces(const char *s)
{
	int		start;
	int		end;
	char	*trim;
	int		i;

	if (!s)
		return (NULL);
	start = 0;
	while (s[start] && s[start] == ' ')
		start++;
	end = ft_strlen(s) - 1;
	while (end >= start && s[end] == ' ')
		end--;
	if (end < start)
		return (ft_strdup(""));
	trim = malloc(end - start + 2);
	if (!trim)
		return (NULL);
	i = 0;
	while (start <= end)
		trim[i++] = s[start++];
	trim[i] = '\0';
	return (trim);
}

void	remove_extra_spaces(char *new_str, const char *str, int *j)
{	
	int		i;
	int		in_space;

	i = 0;
	in_space = 0;
	while (str[i])
	{
		if (str[i] == ' ')
		{
			if (!in_space)
			{
				new_str[(*j)++] = ' ';
				in_space = 1;
			}
		}
		else
		{
			new_str[(*j)++] = str[i];
			in_space = 0;
		}
		i++;
	}
}

char	*compress_whitespace(const char *str)
{
	char	*new_str;
	int		j;

	if (!str)
		return (NULL);
	j = 0;
	if (!str)
		return (NULL);
	new_str = malloc(ft_strlen(str) + 1);
	if (!new_str)
		return (NULL);
	remove_extra_spaces(new_str, str, &j);
	new_str[j] = '\0';
	return (new_str);
}

