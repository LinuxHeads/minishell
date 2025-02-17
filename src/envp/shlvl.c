/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:35:29 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/17 13:08:27 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	ft_setup_value(t_env **envp, int shlvl)
{
	char	*new_shlvl;

	if (shlvl >= 1000)
	{
		printf("minishell: warning: shell \
level (SHLVL) too high,resetting to 1\n");
		if (!ft_setenv("SHLVL", "1", envp))
			return (0);
		return (1);
	}
	if (shlvl < 0)
	{
		if (!ft_setenv("SHLVL", "0", envp))
			return (0);
		return (1);
	}
	new_shlvl = ft_itoa(shlvl + 1);
	if (!new_shlvl)
		return (0);
	if (!ft_setenv("SHLVL", new_shlvl, envp))
	{
		free(new_shlvl);
		return (0);
	}
	free(new_shlvl);
	return (1);
}

static int	ft_check_errors(int shlvl, char *shlvl_str, t_env **envp)
{
	if (shlvl == 0 && shlvl_str[0] != '0')
	{
		if (!ft_setenv("SHLVL", "0", envp))
			return (0);
		return (2);
	}
	if (!ft_isnumber(shlvl_str))
	{
		if (!ft_setenv("SHLVL", "1", envp))
			return (0);
		return (2);
	}
	return (1);
}

int	ft_setup_shlvl(t_env **envp)
{
	int		shlvl;
	char	*shlvl_str;

	if (!envp || !*envp)
		return (0);
	shlvl_str = ft_getenv("SHLVL", *envp);
	if (!shlvl_str)
	{
		if (!ft_setenv("SHLVL", "1", envp))
			return (0);
		return (1);
	}
	shlvl = ft_atoi(shlvl_str);
	if (!ft_check_errors(shlvl, shlvl_str, envp))
		return (0);
	if (ft_check_errors(shlvl, shlvl_str, envp) == 2)
		return (1);
	if (!ft_setup_value(envp, shlvl))
		return (0);
	return (1);
}

