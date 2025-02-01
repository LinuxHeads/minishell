/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:41:43 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/01 23:37:55 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void ft_cd(char *args, t_env **envp)
{
  char *path;
  char cwd[1024];
  char *oldpwd;

  // If no arguments, use $HOME
  printf("%s\n", args);
  args = ft_strtrim(args, " ");
  if (args == NULL || args[0] == '\0')
  {
    path = ft_getenv("HOME", *envp);
    if (path == NULL)
    {
      write(2, "cd: HOME not set\n", 18);
      return;
    }
  }
  // If argument is "-", use $OLDPWD
  else if (ft_strcmp(args, "-") == 0)
  {
    path = ft_getenv("OLDPWD", *envp);
    if (path == NULL)
    {
      write(2, "cd: OLDPWD not set\n", 19);
      return;
    }
    printf("%s\n", path);  // Print the directory to change to
  }
  else
  {
    path = args;  // Use the provided argument
  }

  // Save the current directory to OLDPWD
  if (!getcwd(cwd, sizeof(cwd)))
  {
    perror("getcwd");
    return;
  }
  oldpwd = ft_strdup(cwd);  // Remember the current directory
  if (!oldpwd)
  {
    perror("strdup");

    return;
  }
  // Attempt to change the directory
  if (chdir(path) < 0)
  {
    perror("cd");
    free(oldpwd);  // Free allocated memory for oldpwd
    return;
  }

  // Update OLDPWD and PWD
  ft_setenv("OLDPWD", oldpwd, envp);
  if (getcwd(cwd, sizeof(cwd)))
  {
    ft_setenv("PWD", cwd, envp);
  }

  free(oldpwd);  // Free allocated memory for oldpwd    }
}
