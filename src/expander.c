/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 01:23:07 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/06 07:01:00 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdio.h>

char	*expand_variable(const char *str, int *index, t_shell *shell)
{
	int		start;
	char	*var_name;
	char	*expanded;

	if (str[*index] == '?')
	{
		(*index)++;
		return (ft_itoa(shell->exit_status));
	}
	start = *index;
	while (str[*index] && (ft_isalnum(str[*index]) || str[*index] == '_'))
		(*index)++;
	if (start == *index)
		return (ft_strdup("$"));
	var_name = ft_substr(str, start, *index - start);
	if (!var_name)
		return (NULL);
	char *env_val = ft_getenv(var_name, shell->env_list);
	if (!env_val)
		env_val = "";  // Use an empty string if the variable is not found
	expanded = ft_strdup(env_val);
	free(var_name);
	return (expanded);
}

char	*expand_string(const char *str, t_shell *shell)
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
			var_value = expand_variable(str, &i, shell);
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
			char *tmp = ft_strjoin(result, var_value);
			free(result);
			result = tmp;
			free(var_value);
		}
		i++;
	}
	return (result);
}

void	expander(char ***argv_ptr, t_shell *shell)
{
	char	**argv;
	int		i;
	char	*expanded;

	argv = *argv_ptr;
	i = 0;
	while (argv[i] != NULL)
	{
		expanded = expand_string(argv[i], shell);
		free(argv[i]);
		if (!expanded)
		{
			*argv_ptr = NULL;
			return ;
		}
		size_t len = ft_strlen(expanded);
		if (len >= 2 &&
			((expanded[0]=='\"' && expanded[len-1]=='\"') ||
			(expanded[0]=='\'' && expanded[len-1]=='\'')))
		{
			char *tmp = ft_substr(expanded, 1, len - 2);
			free(expanded);
			expanded = tmp;
		}

		argv[i] = expanded;
		i++;
	}
}
