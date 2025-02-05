/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:40:45 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/05 14:36:59 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>

int ft_echo(char **args)
{
    int i = 1;
    int newline = 1;

    while (args[i] && strcmp(args[i], "-n") == 0)
    {
        newline = 0;
        i++;
    }

    while (args[i])
    {
        printf("%s", args[i]);
        if (args[i + 1])
            printf(" ");
        i++;
    }

    if (newline)
	{
        printf("\n");
	}
	return (1);
}
