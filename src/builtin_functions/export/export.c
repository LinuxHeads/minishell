/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:42:39 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/21 23:58:19 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	print_export(t_env *env)
{
	while (env)
	{
		printf("declare -x %s", env->name);
		if (env->value != NULL)
			printf("=\"%s\"", env->value);
		printf("\n");
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
    
    if (!ft_setenv(key, value, env_list))
    {
        free(key);
        free(value);
        return (1);
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
		else if (status == 1 && ret != 2)
			ret = 1;
		i++;
	}
	return (ret);
}
