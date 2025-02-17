/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 05:18:53 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/17 05:19:18 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	split_with_equal(const char *arg, char **key, char **value)
{
	int	key_len;

	key_len = ft_strchr(arg, '=') - arg;
	*key = ft_substr(arg, 0, key_len);
	if (!*key)
		return (0);
	*value = ft_strdup(ft_strchr(arg, '=') + 1);
	if (!*value)
	{
		free(*key);
		return (0);
	}
	return (1);
}

int	split_without_equal(const char *arg, char **key, char **value)
{
	*key = ft_strdup(arg);
	if (!*key)
		return (0);
	*value = NULL;
	return (1);
}

int	split_var(const char *arg, char **key, char **value)
{
	char	*equal_sign;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
		return (split_with_equal(arg, key, value));
	else
		return (split_without_equal(arg, key, value));
}
