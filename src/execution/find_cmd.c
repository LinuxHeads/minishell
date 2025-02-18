/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 06:21:46 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/18 12:17:41 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*search_executable_in_paths(char **cmd, char **paths,
		char **full_path, int *i)
{
	char *path;
	
	while (paths[*i])
	{
		*full_path = ft_strjoin(paths[*i], "/");
		if (!full_path)
			return (NULL);
		path = ft_strjoin(*full_path, *cmd);
		if (!path)
			return (NULL);
		free(*full_path);
		*full_path = path;
		if (access(*full_path, X_OK) == 0)
		{
			free_str_array(paths);
			return (*full_path);
		}
		free(*full_path);
		(*i)++;
	}
	return (NULL);
}

char	*locate_command_in_env(char **envp, char *cmd)
{
	char	*path_var;
	char	**paths;
	char	*full_path;
	int		i;

	i = -1;
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_var = envp[i] + 5;
			paths = ft_split(path_var, ':');
			if (!paths)
			{
				return (NULL);
			}
			i = 0;
			full_path = search_executable_in_paths(&cmd, paths, &full_path, &i);
			if (full_path)
				return (full_path);
			free_str_array(paths);
			return (NULL);
		}
	}
	return (NULL);
}

char	*find_command_path(char *cmd, char **envp)
{
	char	*path_var;
	char	*path;

	if (ft_strchr(cmd, '/'))
	{
		path_var = ft_strdup(cmd);
		if (!path_var)
			return (NULL);
		if (access(path_var, X_OK) == 0)
			return (path_var);
		free(path_var);
		return (NULL);
	}
	if (access(cmd, X_OK) == 0)
	{
		path_var = ft_strdup(cmd);
		if (!path_var)
			return (NULL);
		return (path_var);
	}
	path = locate_command_in_env(envp, cmd);
	if (path)
		return (path);
	return (NULL);
}
