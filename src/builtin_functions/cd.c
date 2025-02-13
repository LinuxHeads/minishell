/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:33:45 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/13 05:09:07 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	validate_cd_args(char **args)
{
	if (args && args[0] && args[1])
	{
		write(2, "cd: too many arguments\n", 24);
		return (0);
	}
	return (1);
}

static char	*get_target_path(char **args, t_env **envp)
{
	char	*path;

	if (!args || !args[0] || ft_strcmp(args[0], "~") == 0 || ft_strcmp(args[0],
			"--") == 0)
	{
		path = ft_getenv("HOME", *envp);
		if (!path)
		{
			write(2, "cd: HOME not set\n", 17);
			return (NULL);
		}
		return (path);
	}
	else if (ft_strcmp(args[0], "-") == 0)
	{
		path = ft_getenv("OLDPWD", *envp);
		if (!path)
		{
			write(2, "cd: OLDPWD not set\n", 19);
			return (NULL);
		}
		return (path);
	}
	return (args[0]);
}

static char	*save_current_directory(void)
{
	char	cwd[1024];
	char	*oldpwd;

	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("getcwd");
		return (NULL);
	}
	oldpwd = ft_strdup(cwd);
	if (!oldpwd)
		perror("ft_strdup");
	return (oldpwd);
}

static int	update_env_variables(t_env **envp, char *oldpwd, char *path)
{
	char	cwd[1024];

	if (!ft_setenv("OLDPWD", oldpwd, envp))
	{
		free(oldpwd);
		return (0);
	}
	if (!getcwd(cwd, sizeof(cwd)))
	{
		if (!ft_setenv("PWD", path, envp))
		{
			free(oldpwd);
			return (0);
		}
	}
	else
	{
		if (!ft_setenv("PWD", cwd, envp))
		{
			free(oldpwd);
			return (0);
		}
	}
	return (1);
}

int	ft_cd(char **args, t_env **envp)
{
	char	*path;
	char	*oldpwd;

	if (!validate_cd_args(args))
		return (1);
	// this will be added to the expander function	later, it will fix leakes and some shit
	args[0] = ft_str_replace(args[0], "~", ft_getenv("HOME", *envp));
	if (!args[0])
		return (1);
	path = get_target_path(args, envp);
	if (!path)
		return (1);
	oldpwd = save_current_directory();
	if (!oldpwd)
		return (1);
	if (chdir(path) != 0)
	{
		perror("cd");
		free(oldpwd);
		return (1);
	}
	if (args && args[0] && ft_strcmp(args[0], "-") == 0)
		printf("%s\n", path);
	if (!update_env_variables(envp, oldpwd, path))
		return (1);
	free(oldpwd);
	return (0);
}

// test leaks for cd
// int main(int ac, char **av, char **env)
// {
	// t_env *envp = init_envp(env);
	// char *args[] = {"/", NULL};
	// 
	// ft_cd(args, &envp);
	// free_envp_list(envp);
	// return 0;
// }
