/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:42:39 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/18 14:43:37 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	print_export(t_env *env)
{
	while (env)
	{
		if (env->value != NULL)
		{
			printf("declare -x ");
			printf("%s=", env->name);
			printf("\"%s\"\n", env->value);
		}
		env = env->next;
	}
}

static int	ft_printenv_sorted(t_env *env_list)
{
	t_env	*tmp;

	tmp = ft_copy_env(env_list);
	if (!tmp)
		return (0);
	ft_sort_env(&tmp);
	print_export(tmp);
	free_envp_list(tmp);
	return (1);
}

static int	process_export_argument(const char *arg, t_env **env_list)
{
	char	*key;
	char	*value;
	int		syntax;

	syntax = export_syntax_error(arg);
	if (syntax != 1)
	{
		if (syntax == 2)
			return (2);
		return (1);
	}
	if (!split_var(arg, &key, &value))
		return (1);
	if (value)
	{
		if (set_or_append_env(key, value, env_list))
		{
			free(key);
			free(value);
			return (1);
		}
	}
	free(key);
	free(value);
	return (0);
}

int	ft_export(char **args, t_env **env_list)
{
	int	i;
	int	ret;
	int	status;

	i = 0;
	ret = 0;
	if (!args[0])
		return (!ft_printenv_sorted(*env_list));
	while (args[i])
	{
		status = process_export_argument(args[i], env_list);
		if (status == 2)
			ret = 2;
		else if (status == 1 && ret != 2)  // Only set ret to 1 if no option error occurred.
			ret = 1;
		i++;
	}
	return (ret);
}
