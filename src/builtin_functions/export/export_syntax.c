/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 05:15:07 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/18 17:47:01 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	validate_identifier_chars(const char *arg, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
		{
			if (arg[i] == '+' && arg[i + 1] == '=')
				i++;
			else
			{
				ft_putstr_fd("minishell: export: `", STDERR_FILENO);
				ft_putstr_fd(arg, STDERR_FILENO);
				ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
				return (0);
			}
		}
		i++;
	}
	return (1);
}

int	check_initial_syntax(const char *arg)
{
	if (ft_isdigit(arg[0]) || arg[0] == '=' || arg[0] == '\0')
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		return (0);
	}
	if (arg[0] == '-' || (arg[0] == '+' && ft_isalpha(arg[1])))
	{
		ft_putstr_fd("minishell: export: ", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(": invalid option\n", STDERR_FILENO);
		ft_putstr_fd("export: usage: export [-fn] [name[=value] ...] or export \
-p\n", STDERR_FILENO);
		return (2);
	}
	return (1);
}

int	export_syntax_error(const char *arg)
{
	int			status;
	const char	*equal_sign;
	size_t		len;

	status = check_initial_syntax(arg);
	if (status != 1)
		return (status);
	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
		len = equal_sign - arg;
	else
		len = ft_strlen(arg);
	return (validate_identifier_chars(arg, len));
}
