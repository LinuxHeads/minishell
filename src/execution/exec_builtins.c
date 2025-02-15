/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 04:11:00 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/15 05:58:59 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_builtin_command(char **arg)
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

int	execute_builtin_command(char **args, t_shell *shell)
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
