/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:42:39 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/16 18:13:13 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	ft_printenv_sorted(t_env *env_list)
{
	t_env	*tmp;

	tmp = ft_copy_env(env_list);
	if (!tmp)
		return (0);
	ft_sort_env(&tmp);
	print_envp(tmp);
	free_envp_list(tmp);
	return (1);
}

static int	check_initial_syntax(const char *arg)
{
	if (ft_isdigit(arg[0]) || arg[0] == '=' || arg[0] == '\0' || arg[0] == '+'
		|| arg[0] == '-')
	{
		if (arg[0] == '-')
		{
			fprintf(stderr, "minishell: export: `%s': not a valid identifier\n",
				arg);
			return (2);
		}
		fprintf(stderr, "minishell: export: `%s': not a valid identifier\n",
			arg);
		return (0);
	}
	return (1);
}

static int	validate_identifier_chars(const char *arg, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
		{
			if (arg[i] == '+' && arg[i + 1] == '=')
				i++;
			else
			{
				fprintf(stderr,
					"minishell: export: `%s': not a valid identifier\n", arg);
				return (0);
			}
		}
		i++;
	}
	return (1);
}

static int	syntax_error(const char *arg)
{
	int			status;
	const char	*equal_sign;
	size_t		len;

	status = check_initial_syntax(arg);
	if (status != 1)
		return (status);
	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
		len = equal_sign - arg;
	else
		len = ft_strlen(arg);
	return (validate_identifier_chars(arg, len));
}

static int	split_with_equal(const char *arg, char **key, char **value)
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

static int	split_without_equal(const char *arg, char **key, char **value)
{
	*key = ft_strdup(arg);
	if (!*key)
		return (0);
	*value = NULL;
	return (1);
}

static int	split_var(const char *arg, char **key, char **value)
{
	char	*equal_sign;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
		return (split_with_equal(arg, key, value));
	else
		return (split_without_equal(arg, key, value));
}

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

int	for_the(char *key, char *value, t_env **env_list)
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

static int	process_export_argument(const char *arg)
{
	char	*key;
	char	*value;
	int		syntax;

	syntax = syntax_error(arg);
	if (!syntax || syntax == 2)
	{
		if (syntax == 2)
			return (2);
		return (1);
	}
	if (!split_var(arg, &key, &value))
		return (1);
	if (value)
	{
	}
	free(key);
	free(value);
	return (0);
}

int	ft_export(char **args, t_env **env_list)
{
	int	i;
	int	ret;

	i = 0;
	if (!args[0])
		return (!ft_printenv_sorted(*env_list));
	while (args[i])
	{
		ret = process_export_argument(args[i]);
		if (ret)
			return (ret);
		i++;
	}
	return (0);
}
