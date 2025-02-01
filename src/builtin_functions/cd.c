/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 23:51:27 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/01 23:55:57 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/minishell.h"

void ft_cd(char *args, t_env **envp)
{
    char *path;
    char cwd[1024];
    char *oldpwd;

    // Check for too many arguments (pseudo-code)
    // if (num_args > 1) { ... error ... }
    if (args == NULL || args[0] == '\0')
    {
        path = ft_getenv("HOME", *envp);
        if (!path)
        {
            write(2, "cd: HOME not set\n", 17);
            return;
        }
    }
    // Handle "cd -" (use $OLDPWD)
    else if (ft_strcmp(args, "-") == 0)
    {
        path = ft_getenv("OLDPWD", *envp);
        if (!path)
        {
            write(2, "cd: OLDPWD not set\n", 19);
            return;
        }
    }
    else
    {
        path = args; // Use the provided path
    }

    // Save current directory to OLDPWD
    if (!getcwd(cwd, sizeof(cwd)))
    {
        perror("getcwd");
        return;
    }
    oldpwd = ft_strdup(cwd);
    if (!oldpwd)
    {
        perror("ft_strdup");
        return;
    }

    // Change directory
    if (chdir(path) != 0)
    {
        perror("cd");
        free(oldpwd);
        return;
    }

    // Print new directory for "cd -"
    if (ft_strcmp(args, "-") == 0)
        printf("%s\n", path);

    // Update OLDPWD and PWD
    ft_setenv("OLDPWD", oldpwd, envp);
    if (!getcwd(cwd, sizeof(cwd)))
    {
        // Fallback to path if getcwd fails
        ft_setenv("PWD", path, envp);
    }
    else
    {
        ft_setenv("PWD", cwd, envp);
    }

    free(oldpwd);
}