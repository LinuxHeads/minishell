/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_set.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 05:23:33 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/21 23:55:01 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

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

int	set_or_append_env(char *key, char *value, t_env **env_list)
{
    // Check if key ends with '+' for append mode
    if (key[ft_strlen(key) - 1] == '+')
    {
        if (!appeand_mode(key, value, env_list))
        {
            free(key);
            free(value);
            return (1);
        }
    }
    else
    {
        if (!ft_setenv(key, value, env_list))
        {
            free(key);
            free(value);
            return (1);
        }
    }
    return (0);
}
