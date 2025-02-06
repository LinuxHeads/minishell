/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:42:39 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/06 07:40:52 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

	if (ft_isdigit(arg[0]) || arg[0] == '=')
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
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
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(arg, 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
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
static int	split_var(const char *arg, char **key, char **value)
{
	char	*equal_sign;
	int		key_len;

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

/**
 * ft_export - Implements the export builtin.
 * @args: Array of arguments (excluding the command name "export").
 * @env_list: Pointer to the environment list.
 *
 * Returns 0 on success, 1 on failure.
 */
int	ft_export(char **args, t_env **env_list) // TO-DO : when no args are passed,
{
	char *key;
	char *value;
	int i;

	i = -1;
	if (!args[0])
	{
		print_envp(*env_list); // should be sorted
		return (0);
	}
	while (args[++i])
	{
		if (syntax_error(args[i]))
		{
			if (!split_var(args[i], &key, &value))
				return (1);
			if (!ft_setenv(key, value, env_list))
			{
				free(key);
				if (value)
					free(value);
				return (1);
			}
			free(key);
			if (value)
				free(value);
		}
		else
			return (1);
	}
	return (0);
}
