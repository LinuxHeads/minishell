/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 06:21:46 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/06 18:51:19 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


static char *find_executable_in_paths(char **cmd, char **paths, char **full_path, int *i)
{
    while (paths[*i])
    {
        *full_path = malloc(ft_strlen(paths[*i]) + ft_strlen(*cmd) + 2);
        if (!*full_path)
            return (NULL);
        sprintf(*full_path, "%s/%s", paths[*i], *cmd);
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
char			*find_command_path(char *cmd, char **envp)
{
    char    *path_var;
    char    **paths;
    char    *full_path;
    int     i;

    if (ft_strchr(cmd, '/'))
        return (ft_strdup(cmd));

    if (access(cmd, X_OK) == 0)
        return (ft_strdup(cmd));

    i = -1;
    while (envp[++i])
    {
        if (ft_strncmp(envp[i], "PATH=", 5) == 0)
        {
            path_var = envp[i] + 5;
            paths = ft_split(path_var, ':');
            if (!paths)
                return (NULL);
            i = 0;
            full_path = find_executable_in_paths(&cmd, paths, &full_path, &i);
            if (full_path)
                return (full_path);
            free_str_array(paths);
            return (NULL);
        }
    }
    return (NULL);
}


int is_builtin(char **arg)
{
	if (ft_strcmp(arg[0], "echo") == 0)
		return (1);
	if (ft_strcmp(arg[0], "cd") == 0)
		return (1);
	if (ft_strcmp(arg[0], "pwd") == 0)
		return (1);
	if (ft_strcmp(arg[0], "export") == 0)
		return (1);
	if (ft_strcmp(arg[0], "unset") == 0)
		return (1);
	if (ft_strcmp(arg[0], "env") == 0)
		return (1);
	if (ft_strcmp(arg[0], "exit") == 0)
		return (1);
	return (0);

}



int exec_builtins(char **args, t_shell *shell)
{
	if (ft_strcmp(args[0], "echo") == 0)
		return (ft_echo(args));
	if (ft_strcmp(args[0], "cd") == 0)
		return (ft_cd(args + 1, &shell->env_list));
	if (ft_strcmp(args[0], "pwd") == 0)
		return (ft_pwd());
	if (ft_strcmp(args[0], "export") == 0)
		return (ft_export(args + 1, &shell->env_list));
	if (ft_strcmp(args[0], "unset") == 0)
		return (ft_unset(args + 1, &shell->env_list));
	if (ft_strcmp(args[0], "env") == 0)
		return (ft_env(args + 1, shell));
	if (ft_strcmp(args[0], "exit") == 0)
		return (ft_exit(args + 1, shell));
	return (0);

}