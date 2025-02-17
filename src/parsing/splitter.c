/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitter.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 07:05:40 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/13 03:57:28 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	skip_quote(const char **s)
{
	char	quote;

	quote = **s;
	(*s)++;
	while (**s && **s != quote)
		(*s)++;
	if (**s)
		(*s)++;
}

static size_t	get_sep(const char *s, char c)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (!*s)
			break ;
		count++;
		while (*s && *s != c)
		{
			if (*s == '"' || *s == '\'')
				skip_quote(&s);
			else
				s++;
		}
	}
	return (count);
}

static char	*extract_word(const char *start, const char *end)
{
	size_t	len;
	char	*word;
	size_t	i;

	len = end - start;
	word = malloc(len + 1);
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = start[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static char	*get_token(const char **s, char c)
{
	const char	*start;
	char		*word;

	while (**s && **s == c)
		(*s)++;
	if (!**s)
		return (NULL);
	start = *s;
	while (**s && **s != c)
	{
		if (**s == '"' || **s == '\'')
			skip_quote(s);
		else
			(*s)++;
	}
	word = extract_word(start, *s);
	return (word);
}

char	**ft_splitter(const char *s, char c)
{
	char		**result;
	size_t		count;
	size_t		index;
	const char	*ptr;
	char		*token;

	if (!s)
		return (NULL);
	count = get_sep(s, c);
	result = malloc((count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	index = 0;
	ptr = s;
	while (1)
	{
		token = get_token(&ptr, c);
		if (!token)
			break ;
		result[index++] = token;
	}
	result[index] = NULL;
	return (result);
}

// int main()
// {
// char	**commands;
// char	*input = "ls -l | cat -e | wc -l";
// commands = ft_splitter(input, '|');
// for (int i = 0; commands[i]; i++)
// {
// printf("%s\n", commands[i]);
// }
// free_str_array(commands);
// return (0);
// }
