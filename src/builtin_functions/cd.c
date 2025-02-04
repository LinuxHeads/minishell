/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 23:51:27 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/04 04:18:35 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int ft_cd(char **args, t_env **envp)
{
    char *path;
    char cwd[1024];
    char *oldpwd;

    if (args && args[0] && args[1])
    {
        write(2, "cd: too many arguments\n", 24);
        return (1);
    }
    if (args == NULL || args[0] == NULL)
    {
        path = ft_getenv("HOME", *envp);
        if (!path)
        {
            write(2, "cd: HOME not set\n", 17);
            return (1);
        }
    }
    else if (ft_strcmp(args[0], "-") == 0)
    {
        path = ft_getenv("OLDPWD", *envp);
        if (!path)
        {
            write(2, "cd: OLDPWD not set\n", 19);
            return (1);
        }
    }
    else
        path = args[0];
    if (!getcwd(cwd, sizeof(cwd)))
    {
        perror("getcwd");
        return (1);
    }
    oldpwd = ft_strdup(cwd);
    if (!oldpwd)
    {
        perror("ft_strdup");
        return (1);
    }
    if (chdir(path) != 0)
    {
        perror("cd");
        free(oldpwd);
        return (1);
    }
    if (args && args[0] && ft_strcmp(args[0], "-") == 0)
        printf("%s\n", path);
    if (!ft_setenv("OLDPWD", oldpwd, envp))
    {
        free(oldpwd);
        return (1);
    }
    if (!getcwd(cwd, sizeof(cwd)))
    {
        if (!ft_setenv("PWD", path, envp))
        {
            free(oldpwd);
            return (1);
        }
    }
    else
    {
        if (!ft_setenv("PWD", cwd, envp))
        {
            free(oldpwd);
            return (1);
        }
    }
    free(oldpwd);
    return (0);
}