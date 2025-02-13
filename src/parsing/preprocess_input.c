/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess_input.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 02:35:03 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/13 03:46:10 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*preprocess_input(char *input)
{
	char	*new_input;
	char	*tmp;

	if (!input)
		return (NULL);
	new_input = ft_str_replace(input, ">", " > ");
	if (!new_input)
		return (NULL);
	tmp = new_input;
	new_input = ft_str_replace(tmp, "<", " < ");
	if (!new_input)
		return (NULL);
	free(tmp);
	tmp = new_input;
	new_input = ft_str_replace(tmp, " >  > ", " >> ");
	if (!new_input)
		return (NULL);
	free(tmp);
	tmp = new_input;
	new_input = ft_str_replace(tmp, " <  < ", " << ");
	if (!new_input)
		return (NULL);
	free(tmp);
	tmp = new_input;
	new_input = ft_str_replace(tmp, "	", " ");
	if (!new_input)
		return (NULL);
	free(tmp);
	tmp = new_input;
	new_input = ft_str_replace(tmp, "|", " | ");
	if (!new_input)
		return (NULL);
	free(tmp);
	return (new_input);
}
