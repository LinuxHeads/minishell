/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_replace.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 12:45:54 by ahramada          #+#    #+#             */
/*   Updated: 2025/02/14 12:47:26 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
static int	count_occurrences(const char *str, const char *old)
{
	int	count;
	int	i;
	int	old_len;

	count = 0;
	i = 0;
	old_len = ft_strlen(old);
	while (str[i])
	{
		if (ft_strncmp(&str[i], old, old_len) == 0)
		{
			count++;
			i += old_len;
		}
		else
			i++;
	}
	return (count);
}

static void	replace_occurrences(char *result, const char *str, const char *old,
		const char *new)
{
	int	i;
	int	j;
	int	old_len;
	int	new_len;

	i = 0;
	j = 0;
	old_len = ft_strlen(old);
	new_len = ft_strlen(new);
	while (str[i])
	{
		if (ft_strncmp(&str[i], old, old_len) == 0)
		{
			ft_memcpy(&result[j], new, new_len);
			j += new_len;
			i += old_len;
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
}

char	*ft_str_replace(const char *str, const char *old, const char *new)
{
	char	*result;
	int		count;
	int		new_str_size;
	
	if (!str || !old || !new || *old == '\0')
		return (ft_strdup(str));
  
	count = count_occurrences(str, old);
	if (count == 0)
		return (ft_strdup(str));
  
	new_str_size = ft_strlen(str) + count * (ft_strlen(new) - ft_strlen(old))
		+ 1;
	result = malloc(new_str_size);
	if (!result)
		return (NULL);
	replace_occurrences(result, str, old, new);
	return (result);
}
