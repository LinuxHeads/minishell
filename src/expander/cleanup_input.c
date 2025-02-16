/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 07:39:53 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/16 07:40:10 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*cleanup_input(char *input)
{
	char	*new_input;
	char	*tmp;

	if (!input)
		return (NULL);
	new_input = ft_str_replace(input, " > ", ">");
	if (!new_input)
		return (NULL);
	tmp = new_input;
	new_input = ft_str_replace(tmp, " < ", "<");
	free(tmp);
	tmp = new_input;
	new_input = ft_str_replace(tmp, " >> ", ">>");
	free(tmp);
	tmp = new_input;
	new_input = ft_str_replace(tmp, " << ", "<<");
	free(tmp);
	tmp = new_input;
	new_input = ft_str_replace(tmp, " |", "|");
	free(tmp);
	tmp = new_input;
	new_input = ft_str_replace(tmp, "| ", "|");
	free(tmp);
	return (new_input);
}
