/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_set.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 05:23:33 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/17 05:25:46 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/*
** appeand_mode: if key ends with '+' then append value to the old value.
*/
int	appeand_mode(char *key, char *value, t_env **env_list)
{
	char	*old_value;
	char	*new_value;

	if (key[ft_strlen(key) - 1] == '+')
	{
		key[ft_strlen(key) - 1] = '\0';
		old_value = ft_getenv(key, *env_list);
		if (old_value)
			new_value = ft_strjoin(old_value, value);
		else
			new_value = ft_strdup(value);
		if (!new_value || !ft_setenv(key, new_value, env_list))
		{
			free(new_value);
			return (0);
		}
		free(new_value);
	}
	return (1);
}

/*
** set_or_append_env: Wraps appeand_mode and ft_setenv.
*/
int	set_or_append_env(char *key, char *value, t_env **env_list)
{
	if (!appeand_mode(key, value, env_list))
	{
		free(key);
		free(value);
		return (1);
	}
	else if (!ft_setenv(key, value, env_list))
	{
		free(key);
		free(value);
		return (1);
	}
	return (0);
}
