/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:40:45 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/10 17:06:18 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


int check_1(char *s);
int check(char *s);

int	ft_echo(char **args)
{
	int	i;
	int	newline_1;

	i = 1;
	newline_1 = 1;
	if(!args[i])
		return 0;
	while (args[i] && ft_strcmp(args[i], "-n") == 0)
	{
		newline_1 = 0;
		i++;
	}
	if(!args[i])
	{
	if (newline_1)
	{
		printf("\n");
	}
		return 0;
	}
	if (check(args[i]) || check_1(args[i]))
	{
		if (newline_1)
			printf("\n");
			return (0);
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline_1)
	{
		printf("\n");
	}
	return (0);
}
