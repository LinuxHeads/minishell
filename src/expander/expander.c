/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 01:23:07 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/08 13:55:42 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_strisspace(const char *str)
{
	int	i;

	if (!str || !*str)
		return (1);
	i = 0;
	while (str[i])
	{
		if (str[i] != ' ' && (str[i] < '\t' || str[i] > '\r'))
			return (0);
		i++;
	}
	return (1);
}

static char	*ft_strtrim_spaces(const char *s)
{
	int		start;
	int		end;
	char	*trim;
	int		i;

	if (!s)
		return (NULL);
	start = 0;
	while (s[start] && s[start] == ' ')
		start++;
	end = ft_strlen(s) - 1;
	while (end >= start && s[end] == ' ')
		end--;
	if (end < start)
		return (ft_strdup(""));
	trim = malloc(end - start + 2);
	if (!trim)
		return (NULL);
	i = 0;
	while (start <= end)
		trim[i++] = s[start++];
	trim[i] = '\0';
	return (trim);
}

char	*compress_spaces(const char *str)
{
	char	*new_str;
	int		i;
	int		j;
	int		in_space;

	if (!str)
		return (NULL);
	new_str = malloc(ft_strlen(str) + 1);
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	in_space = 0;
	while (str[i])
	{
		if (str[i] == ' ')
		{
			if (!in_space)
			{
				new_str[j++] = ' ';
				in_space = 1;
			}
		}
		else
		{
			new_str[j++] = str[i];
			in_space = 0;
		}
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

static void	remove_arg(char ***argv_ptr, int index)
{
	char	**argv;
	int		i;

	argv = *argv_ptr;
	i = index;
	free(argv[i]);
	while (argv[i + 1])
	{
		argv[i] = argv[i + 1];
		i++;
	}
	argv[i] = NULL;
}

char	*expand_variable(const char *str, int *index, t_shell *shell)
{
	int		start;
	char	*var_name;
	char	*env_val;
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
	env_val = ft_getenv(var_name, shell->env_list);
	free(var_name);
	if (!env_val)
		env_val = "";
	expanded = ft_strdup(env_val);
	return (expanded);
}

char *remove_closed_quotes(const char *s)
{
    size_t len;
    size_t i;
    size_t j;
    char   *res;

    if (!s)
        return NULL;
    len = strlen(s);
    res = malloc(len + 1);
    if (!res)
        return NULL;

    i = 0;
    j = 0;
    while (i < len)
    {
        if (s[i] == '\'' || s[i] == '"')
        {
            char quote = s[i];
            size_t k = i + 1;
            while (k < len && s[k] != quote)
                k++;
            if (k < len && s[k] == quote)
            {
                i++;
                while (i < k)
                    res[j++] = s[i++];
                i = k + 1;
            }
            else
            {
                res[j++] = s[i++];
            }
        }
        else
        {
            res[j++] = s[i++];
        }
    }
    res[j] = '\0';
    return res;
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
		if (str[i] == '\'' && !in_dq)
			in_sq = !in_sq;
		else if (str[i] == '"' && !in_sq)
			in_dq = !in_dq;
		else if (str[i] == '$' && !in_sq)
		{
			i++;
			var_value = expand_variable(str, &i, shell);
			if (var_value)
			{
				if (!in_dq)
				{
					char *trim = ft_strtrim_spaces(var_value);
					//printf("%s\n",trim);
					free(var_value);
					if (!trim)
					{
						free(result);
						return (NULL);
					}
					if (ft_strisspace(trim))
					{
						free(trim);
						continue ;
					}
					var_value = compress_spaces(trim);
					free(trim);
					if (!var_value)
					{
						free(result);
						return (NULL);
					}
				}
				tmp = ft_strjoin(result, var_value);
				free(var_value);
				if (!tmp)
				{
					free(result);
					return (NULL);
				}
				free(result);
				result = tmp;
			}
			continue ;
		}
		else
		{
			char c[2];
			c[0] = str[i];
			c[1] = '\0';
			tmp = ft_strjoin(result, c);
			if (!tmp)
			{
				free(result);
				return (NULL);
			}
			free(result);
			result = tmp;
		}
		i++;
	}
	return (result);
}

char	*preprocess_input_test(char *input)
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
	// tmp = new_input;
	// new_input = ft_str_replace(tmp, " $", "$");
	// free(tmp);
	return (new_input);
}


int	expander(char ***argv_ptr, t_shell *shell)
{
	char	**argv;
	char	*expanded;
	char	*tmp;
	char	*old_arg;
	size_t	len;
	int		i;

	argv = *argv_ptr;
	i = 0;
	while (argv[i])
	{
		old_arg = argv[i];
		if (!ft_strchr(old_arg, '$'))
		{
			i++;
			continue;
		}
		expanded = expand_string(old_arg, shell);
		if (!expanded || ft_strisspace(expanded))
		{
			argv[i] = NULL;
			//remove_arg(&argv, i);
			// return 0;
		}
		len = ft_strlen(expanded);
		if (len >= 2 && (
			(expanded[0] == '\"' && expanded[len - 1] == '\"')
			|| (expanded[0] == '\'' && expanded[len - 1] == '\'')
		))
		{
			tmp = ft_substr(expanded, 1, len - 2);
			free(expanded);
			if (!tmp)
			{
				argv[i] = NULL;
				//remove_arg(&argv, i);
				// return 0;
			}
			expanded = tmp;
		}
		if (ft_strisspace(expanded))
		{
			free(expanded);
			remove_arg(&argv, i);
			continue ;
		}
		free(old_arg);
		argv[i] = preprocess_input_test(expanded);
		free(expanded);
		i++;
	}
	*argv_ptr = argv;
	i = 0;
	int check=0;
	while (argv[i])
	{
		char *no_closed_quotes = remove_closed_quotes(argv[i]);
		if (no_closed_quotes)
		{
			free(argv[i]);
			argv[i] = no_closed_quotes;
		}
		argv[i] = preprocess_input_test(argv[i]);
		check=1;
		i++;
	}
	return check;
}

void	expander_test(char **argv, t_shell *shell)
{
	char	*expanded;
	size_t	len;

	if (!ft_strchr(*argv, '$'))
	{
		char *no_closed_quotes = remove_closed_quotes(*argv);
		if (no_closed_quotes)
		{
			free(*argv);
			*argv = no_closed_quotes;
		}
		return ;
	}
	expanded = expand_string(*argv, shell);
	free(*argv);
	if (!expanded)
		return ;
	len = ft_strlen(expanded);
	if (len >= 2 && (
		(expanded[0] == '\"' && expanded[len - 1] == '\"')
		|| (expanded[0] == '\'' && expanded[len - 1] == '\'')
	))
	{
		char *tmp = ft_substr(expanded, 1, len - 2);
		free(expanded);
		if (!tmp)
			return ;
		expanded = tmp;
	}
	if (ft_strisspace(expanded))
	{
		free(expanded);
		*argv = NULL;
		return ;
	}
	*argv = preprocess_input_test(expanded);
	free(expanded);
	if (*argv)
	{
		char *no_closed_quotes = remove_closed_quotes(*argv);
		if (no_closed_quotes)
		{
			free(*argv);
			*argv = no_closed_quotes;
		}
	}
}


// void	expander_test(char **argv, t_shell *shell)
// {

// 	char	*expanded;
// 	char	*tmp;

// 	size_t	len;



// 		if (!ft_strchr(argv[0], '$'))
// 		{
// 			return;
// 		}
// 		expanded = expand_string(argv[0], shell);
// 		if (!expanded || ft_strisspace(expanded))
// 		{
// 			argv[0]= NULL;
// 			//remove_arg(&argv, i);
// 			return;
// 		}
// 		len = ft_strlen(expanded);
// 		if (len >= 2 && (
// 			(expanded[0] == '\"' && expanded[len - 1] == '\"')
// 			|| (expanded[0] == '\'' && expanded[len - 1] == '\'')
// 		))
// 		{
// 			tmp = ft_substr(expanded, 1, len - 2);
// 			free(expanded);
// 			if (!tmp)
// 			{
// 				argv[0]= NULL;
// 				//remove_arg(&argv, i);
// 				return;
// 			}
// 			expanded = tmp;
// 		}
// 		if (ft_strisspace(expanded))
// 		{
// 			free(expanded);
// 			remove_arg(&argv, 0);
// 			return;
// 		}

// 		argv[0] = preprocess_input_test(expanded);
// 		free(expanded);

// 		char *no_closed_quotes = remove_closed_quotes(argv[0]);
// 		if (no_closed_quotes)
// 		{
// 			free(argv[0]);
// 			argv[0]= no_closed_quotes;
// 		}
// 		argv[0]= preprocess_input_test(argv[0]);
		

	
	
// }