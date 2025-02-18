/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 07:39:53 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/18 05:06:48 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*replace_and_free(char *input, const char *old_sub, const char *new_sub)
{
	char	*result;

	result = ft_str_replace(input, old_sub, new_sub);
	free(input);
	return (result);
}

char	*cleanup_input(char *input)
{
	char	*result;

	if (!input)
		return (NULL);
	result = ft_str_replace(input, " > ", ">");
	if (!result)
		return (NULL);
	result = replace_and_free(result, " < ", "<");
	if (!result)
		return (NULL);
	result = replace_and_free(result, " >> ", ">>");
	if (!result)
		return (NULL);
	result = replace_and_free(result, " << ", "<<");
	if (!result)
		return (NULL);
	result = replace_and_free(result, " |", "|");
	if (!result)
		return (NULL);
	result = replace_and_free(result, "| ", "|");
	return (result);
}
