/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:30:45 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/17 14:25:50 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	cheker_for_number(char **args, t_shell *shell)
{
	if (ft_isnumber(args[0]))
	{
		ft_exit_handler(shell, NULL, (char *[]){"exit: too many arguments\n", NULL}, 2);
		return (1);
	}
	else
	{
		ft_exit_handler(shell, NULL, (char *[]){"exit: numeric argument required\n", NULL}, 2);
		return (0);
	}
}

int	ft_exit(char **args, t_shell *shell)
{
	if (args[0] && args[1])
	{
		if (cheker_for_number(args, shell))
			return (1);
	}
	if (args[0])
	{
		if (ft_isnumber(args[0]))
		{
			shell->exit_status = ft_atoi(args[0]) % 256;
			ft_exit_handler(shell, NULL, NULL, shell->exit_status);
			return (0);
		}
		else
		{
			ft_exit_handler(shell, NULL, (char *[]){"exit: numeric argument required\n",NULL}, 2);
			return (2);
		}
	}
	ft_exit_handler(shell, NULL, NULL, shell->exit_status);
	return (0);
}
