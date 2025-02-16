/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess_input.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 02:35:03 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/16 12:57:41 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*do_replacement(char *input, const char *old, const char *new_str)
{
	char	*result;

	result = ft_str_replace(input, old, new_str);
	free(input);
	return (result);
}

char	*preprocess_input(char *input)
{
	char	*res;

	if (!input)
		return (NULL);
	res = ft_str_replace(input, ">", " > ");
	if (!res)
		return (NULL);
	res = do_replacement(res, "<", " < ");
	if (!res)
		return (NULL);
	res = do_replacement(res, " >  > ", " >> ");
	if (!res)
		return (NULL);
	res = do_replacement(res, " <  < ", " << ");
	if (!res)
		return (NULL);
	res = do_replacement(res, "\t", " ");
	if (!res)
		return (NULL);
	res = do_replacement(res, "|", " | ");
	return (res);
}
