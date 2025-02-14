/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:40:45 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/14 15:24:34 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


int check_for_options(char *args)
{
	char *str_cp;
	char *str_cp2;

	str_cp = ft_strdup(args);
	if (!str_cp)
		return (0);
	str_cp2 = ft_strdup(args);
	if (!str_cp2)
	{
		free(str_cp);
		return (0);
	}
	if(ft_strchr(args,'-') && ft_strchr(str_cp,'n') && !ft_strchr(str_cp,' '))
	// if(ft_strchr(args,'-') && ft_strchr(str_cp,'n'))
	{
		if(str_cp2[0]=='-' && str_cp2[1]=='-')
		{
			free(str_cp);
			free(str_cp2);
			return (0);
		}
		if(str_cp2[0]=='n' && str_cp2[1]=='-')
		{
			free(str_cp);
			free(str_cp2);
			return (0);
		}
		free(str_cp);
		free(str_cp2);
		return(1);
	}
	free(str_cp);
	free(str_cp2);
	return (0);
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
		
		if((args[i][0]=='\"' && args[i][1]=='\"') || (args[i][0]=='\"' && args[i][1]=='\"'))
			printf(" ");
		else
		{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		}
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
