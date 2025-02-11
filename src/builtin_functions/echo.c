/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:40:45 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/11 12:50:58 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


int check_for_options(char *args)
{
	char *str_cp=ft_strdup(args);
	if(ft_strchr(args,'-') && ft_strchr(str_cp,'n'))
	{
		return(1);
	}
	return 0;
}
static int	process_flags(char **args, int *start_index)
{
	int	i;
	int	newline;

	newline = 1;
	i = 1;
	newline = 1;
	while (args[i] && check_for_options(args[i]))
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
		return (0);
	newline = process_flags(args, &start_index);
	if (!args[start_index])
	{
		if (newline)
			printf("\n");
		return (0);
	}
	if (check_single_qoutes(args[start_index]) || check_double_qoutes(args[start_index]))
	{
		if (newline)
			printf("\n");
		return (0);
	}
	print_arguments(args, start_index);
	if (newline)
		printf("\n");
	return (0);
}
