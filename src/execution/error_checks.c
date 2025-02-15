/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 04:44:23 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/15 06:00:04 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


void	check_if_directory(char *cmd_path, t_shell **shell)
{
	struct stat	path_stat;

	if (stat(cmd_path, &path_stat) == 0)
	{
		if (S_ISDIR(path_stat.st_mode))
		{
			if (!ft_strchr((*shell)->argv[0], '/'))
			{
				fprintf(stderr, "%s: command not found\n", (*shell)->argv[0]);
				ft_exit_handler(*shell, cmd_path, 0, 127);
			}
			fprintf(stderr, "%s: is a directory\n", cmd_path);
			ft_exit_handler(*shell, cmd_path, 0, 126);
		}
	}
}

void	check_command_permissions(t_shell **shell)
{
	if (access((*shell)->argv[0], F_OK) == 0)
	{
		fprintf(stderr, "%s: permission denied\n", (*shell)->argv[0]);
		ft_exit_handler(*shell, NULL, 0, 126);
	}
	else
	{
		fprintf(stderr, "%s: command not found\n", (*shell)->argv[0]);
		ft_exit_handler(*shell, NULL, 0, 127);
	}
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
			{
				fprintf(stderr, "%s: is a directory\n", ft_getenv("HOME",
						(*shell)->env_list));
				ft_exit_handler(*shell, cmd_path, 0, 126);
			}
			fprintf(stderr, "%s: command not found\n", (*shell)->argv[0]);
			ft_exit_handler(*shell, NULL, 0, 127);
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