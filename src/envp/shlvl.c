/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:35:29 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/06 15:36:01 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


void	ft_setup_shlvl(t_env **envp)
{
	int		shlvl;
	char	*shlvl_str;
	char	*new_shlvl;

	if (!envp || !*envp)
		return ;

	shlvl_str = ft_getenv("SHLVL", *envp);
	if (!shlvl_str)
	{
		ft_setenv("SHLVL", "1", envp);
		return ;
	}

	// Validate SHLVL string (avoid invalid conversions)
	if (!ft_isnumber(shlvl_str))
	{
		ft_setenv("SHLVL", "1", envp);
		return ;
	}

	shlvl = ft_atoi(shlvl_str);
	if (shlvl >= 1000)
	{
		printf("minishell: warning: shell level (SHLVL) too high, resetting to 1\n");
		ft_setenv("SHLVL", "1", envp);
		return ;
	}
	if (shlvl < 0)
	{
		ft_setenv("SHLVL", "0", envp);
		return ;
	}
	new_shlvl = ft_itoa(shlvl + 1);
	if (!new_shlvl)
		return ;
	ft_setenv("SHLVL", new_shlvl, envp);
	free(new_shlvl);
}
