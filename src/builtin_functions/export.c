/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:42:39 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/11 23:06:19 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int ft_printenv_sorted(t_env *env_list)
{
	t_env *tmp;

	tmp = ft_copy_env(env_list);
	if (!tmp)
		return (0);
	ft_sort_env(&tmp);
	print_envp(tmp);
	free_envp_list(tmp);
	return (1);
}

/**
 * syntax_error - Checks if the given argument is a valid identifier.
 * @arg: The argument to check.
 *
 * Returns 1 if valid, 0 if invalid (and prints an error message).
 */
static int	syntax_error(const char *arg)
{
	const char	*equal_sign;
	size_t		len;
	size_t		i;

	if (ft_isdigit(arg[0]) || arg[0] == '=' || arg[0] == '\0' || arg[0] == '+' || arg[0] == '-')
	{
		if (arg[0] == '-')
		{
			fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", arg);
			return (2);
		}
		fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", arg);
		return (0);
	}
	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
		len = equal_sign - arg;
	else
		len = ft_strlen(arg);
	i = -1;
	while (++i < len)
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
		{
			if (arg[i] == '+' && arg[i + 1] == '=')
				continue ;
			fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", arg);
			return (0);
		}
	}
	return (1);
}

/**
 * split_var - Splits the argument into key and value based on '='.
 * @arg: The argument string (e.g., "VAR=value").
 * @key: Pointer to store the duplicated key.
 * @value: Pointer to store the duplicated value.
 *
 * Returns 1 on success, 0 on failure.
 */
static int split_var(const char *arg, char **key, char **value)
{
    char *equal_sign;
    int key_len;

    equal_sign = ft_strchr(arg, '=');
    if (equal_sign)
    {
        key_len = equal_sign - arg;
        *key = ft_substr(arg, 0, key_len);
        if (!*key)
            return (0);
        *value = ft_strdup(equal_sign + 1);
        if (!*value)
        {
            free(*key);
            return (0);
        }
    }
    else
    {
        *key = ft_strdup(arg);
        if (!*key)
            return (0);
		*value = NULL;
	}
    return (1);
}

int	appeand_mode(char *key, char *value, t_env **env_list)
{
	char	*old_value;
	char	*new_value;

	if (key[ft_strlen(key) - 1] == '+') // Handling VAR+=value
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

/**
 * ft_export - Implements the export builtin.
 * @args: Array of arguments (excluding the command name "export").
 * @env_list: Pointer to the environment list.
 *
 * Returns 0 on success, 1 on failure.
 */
int	ft_export(char **args, t_env **env_list)
{
	char	*key;
	char	*value;
	// char	*old_value;
	// char	*new_value;
	int		i;
	int     syntax;
	
	i = -1;
	if (!args[0])
		return (!ft_printenv_sorted(*env_list));
	
	while (args[++i])
	{
		syntax = syntax_error(args[i]);
		if (!syntax || syntax == 2)
		{
			if (syntax == 2)
				return (2);
			return (1);
		}
		if (!split_var(args[i], &key, &value))
			return (1);
		if (value)
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
		}
		free(key);
		free(value);
	}
	return (0);
}
