/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:33:45 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/06 02:58:45 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Helper: Validate the arguments for cd
// Returns 0 if arguments are valid; otherwise, prints an error and returns 1.
static int	validate_cd_args(char **args)
{
	if (args && args[0] && args[1])
	{
		write(2, "cd: too many arguments\n", 24);
		return (0);
	}
	return (1);
}

// Helper: Determine the target path based on args and environment
// Returns a pointer to the target path string (which may come from the environment),
// or NULL if there is an error (and prints an error message).
static char	*get_target_path(char **args, t_env **envp)
{
	char	*path;

	// If no argument is provided, use HOME
	if (!args || !args[0])
	{
		path = ft_getenv("HOME", *envp);
		if (!path)
			write(2, "cd: HOME not set\n", 17);
		return (path);
	}
	// If the argument is "-", use OLDPWD
	else if (ft_strcmp(args[0], "-") == 0)
	{
		path = ft_getenv("OLDPWD", *envp);
		if (!path)
			write(2, "cd: OLDPWD not set\n", 19);
		return (path);
	}
	// Otherwise, the target is the argument itself
	return (args[0]);
}

// Helper: Save current working directory and return a strdup-ed copy
// Returns a new allocated string representing the current directory or NULL on failure.
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

// Helper: Update the environment variables PWD and OLDPWD after changing directory
// Uses 'oldpwd' as the previous working directory and updates PWD to the new cwd.
// If getcwd fails for PWD, it falls back to the 'path' provided.
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
		// If getcwd fails, fall back to using path (though this may be relative)
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

// The main cd function refactored into helper functions
int	ft_cd(char **args, t_env **envp)
{
	char	*path;
	char	*oldpwd;

	if (!validate_cd_args(args))
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
