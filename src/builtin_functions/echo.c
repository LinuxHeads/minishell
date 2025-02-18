/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:40:45 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/18 12:17:02 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_valid_n_flag(const char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i] == 'n')
		i++;
	return (arg[i] == '\0');
}

static int	process_flags(char **args, int *start_index)
{
	int	i;
	int	newline;

	newline = 1;
	i = 1;
	while (args[i] && is_valid_n_flag(args[i]))
	{
		newline = 0;
		i++;
	}
	*start_index = i;
	return (newline);
}

static void	print_arguments(char **args, int start_index)
{
	int	i;

	i = start_index;
	while (args[i])
	{
		if ((args[i][0] == '\"' && args[i][1] == '\"') || (args[i][0] == '\''
				&& args[i][1] == '\''))
			printf(" ");
		else
			printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
}

int	ft_echo(char **args)
{
	int	start_index;
	int	newline;

	if (!args[1])
	{
		printf("\n");
		return (0);
	}
	newline = process_flags(args, &start_index);
	print_arguments(args, start_index);
	if (newline)
		printf("\n");
	return (0);
}
