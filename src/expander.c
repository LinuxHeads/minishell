/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 01:23:07 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/05 15:08:18 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_env_value(char *var)
{
	char	*value;

	value = getenv(var);
	if (value)
		return (value);
	return ("");
}

char	*expand_variable(const char *str, int *index)
{
	int		start;
	char	*var_name;
	char	*expanded;

	if (str[*index] == '?')
		return ("ZOMBI SHOULD HAMDEL IT");
	start = *index;
	while (str[*index] && (ft_isalnum(str[*index]) || str[*index] == '_'))
		(*index)++;
	if (start == *index)
		return (ft_strdup("$"));
	var_name = ft_substr(str, start, *index - start);
	if (!var_name)
		return (NULL);
	expanded = ft_strdup(get_env_value(var_name));
	free(var_name);
	return (expanded);
}

char	*expand_string(const char *str)
{
	char	*result;
	char	*var_value;
	int		i;
	int		in_sq;
	int		in_dq;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	in_sq = 0;
	in_dq = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			in_sq = !in_sq;
		else if (str[i] == '"')
			in_dq = !in_dq;
		else if (str[i] == '$' && !in_sq)
		{
			i++;
			var_value = expand_variable(str, &i);
			if (var_value)
			{
				result = ft_strjoin(result, var_value);
				free(var_value);
			}
			continue ;
		}
		else
		{
			var_value = (char *)malloc(2);
			if (!var_value)
				return (NULL);
			var_value[0] = str[i];
			var_value[1] = '\0';
			result = ft_strjoin(result, var_value);
			free(var_value);
		}
		i++;
	}
	return (result);
}

void	expander(char ***argv_ptr)
{
	char	**argv;
	int		i;
	char	*expanded;

	argv = *argv_ptr;
	i = 0;
	while (argv[i] != NULL)
	{
		expanded = expand_string(argv[i]);
		free(argv[i]);
		argv[i] = expanded;
		i++;
	}
}
