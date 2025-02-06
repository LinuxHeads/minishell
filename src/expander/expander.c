/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 01:23:07 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/06 07:46:47 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*expand_variable(const char *str, int *index, t_shell *shell)
{
	int		start;
	char	*var_name;
	char	*expanded;
	char	*tmp;
	char	*env_val;
	
	if (str[*index] == '?')
	{
		(*index)++;
		tmp = ft_itoa(shell->exit_status);
		if (!tmp)
			return (NULL);
		return (tmp);
	}
	start = *index;
	while (str[*index] && (ft_isalnum(str[*index]) || str[*index] == '_'))
		(*index)++;
	if (start == *index)
	{
		tmp = ft_strdup("$");
		if (!tmp)
			return (NULL);
		return (tmp);
	}
	var_name = ft_substr(str, start, *index - start);
	if (!var_name)
	{
		return (NULL);
	}
	env_val = ft_getenv(var_name, shell->env_list);
	if (!env_val)
		env_val = "";  // Use an empty string if the variable is not found
	expanded = ft_strdup(env_val);
	if (!expanded)
	{
		free(var_name);
		return (NULL);
	}
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
	char	*tmp;
	
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
				tmp = ft_strjoin(result, var_value);
				if (!tmp)
				{
					free(result);
					free(var_value);
					return (NULL);
				}
				free(result);
				result = tmp;
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
			tmp = ft_strjoin(result, var_value);
			if (!tmp)
			{
				free(result);
				free(var_value);
				return (NULL);
			}
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
	size_t len;
	char	*tmp;
	
	argv = *argv_ptr;
	i = 0;
	while (argv[i] != NULL)
	{
		expanded = expand_string(argv[i], shell);
		free(argv[i]);
		if (!expanded)
			return ; // *argv_ptr = NULL was here
		len = ft_strlen(expanded);
		if (len >= 2 &&
			((expanded[0]=='\"' && expanded[len-1]=='\"') ||
			(expanded[0]=='\'' && expanded[len-1]=='\'')))
		{
			tmp = ft_substr(expanded, 1, len - 2);
			free(expanded);
			if (!tmp)
				return ;
			expanded = tmp;
		}
		argv[i] = expanded;
		i++;
	}
}
