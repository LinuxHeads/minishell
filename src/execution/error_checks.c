/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 04:44:23 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/17 15:56:44 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <string.h>

void	check_if_directory(char *cmd_path, t_shell **shell)
{
	struct stat	path_stat;

	if (stat(cmd_path, &path_stat) == 0)
	{
		if (S_ISDIR(path_stat.st_mode))
		{
			if (!ft_strchr((*shell)->argv[0], '/'))
				ft_exit_handler(*shell, cmd_path, (char *[]){(*shell)->argv[0],
					": command not found\n", NULL}, 127);
			ft_exit_handler(*shell, cmd_path, (char *[]){cmd_path,
				": is a directory\n", NULL}, 126);
		}
	}
}

void	check_command_permissions(t_shell **shell)
{
	if (access((*shell)->argv[0], F_OK) == 0)
		ft_exit_handler(*shell, NULL, (char *[]){(*shell)->argv[0],
			": permission denied\n", NULL}, 126);
	else
		ft_exit_handler(*shell, NULL, (char *[]){(*shell)->argv[0],
			": command not found\n", NULL}, 127);
}

void	validate_command_path(char *cmd_path, t_shell **shell)
{
	if (!cmd_path)
	{
		if (ft_strchr((*shell)->argv[0], '/'))
			check_command_permissions(shell);
		else
		{
			if (ft_strcmp((*shell)->argv[0], "~") == 0)
				ft_exit_handler(*shell, cmd_path, (char *[]){ft_getenv("HOME",
						(*shell)->env_list), ": is a directory\n", NULL}, 126);
			ft_exit_handler(*shell, NULL, (char *[]){(*shell)->argv[0],
				": command not found\n", NULL}, 127);
		}
	}
	check_if_directory(cmd_path, shell);
}

void	fork_check(int pid, t_shell **shell)
{
	if (pid < 0)
	{
		perror("fork");
		ft_exit_handler(*shell, NULL, 0, 1);
	}
}
