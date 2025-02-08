/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 01:23:07 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/09 01:09:03 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int		ft_tablen(char **tab);
static char		**replace_token_with_tokens(char **argv, int index,
					char **new_tokens);
static int		is_all_spaces(const char *s);

static int	is_all_spaces(const char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (1);
	while (s[i])
	{
		if (s[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}

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
	{
		trim[i] = s[start];
		i++;
		start++;
	}
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
				new_str[j] = ' ';
				j++;
				in_space = 1;
			}
		}
		else
		{
			new_str[j] = str[i];
			j++;
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

static int	ft_tablen(char **tab)
{
	int	i;

	i = 0;
	while (tab && tab[i])
		i++;
	return (i);
}

static char	**replace_token_with_tokens(char **argv, int index,
					char **new_tokens)
{
	int		old_count;
	int		new_count;
	int		total;
	char	**new_argv;
	int		i;
	int		j;
	int		pos;

	old_count = ft_tablen(argv);
	new_count = ft_tablen(new_tokens);
	total = old_count - 1 + new_count;
	new_argv = malloc(sizeof(char *) * (total + 1));
	if (!new_argv)
		return (NULL);
	pos = 0;
	i = 0;
	while (i < index)
	{
		new_argv[pos] = ft_strdup(argv[i]);
		pos++;
		i++;
	}
	j = 0;
	while (j < new_count)
	{
		new_argv[pos] = ft_strdup(new_tokens[j]);
		pos++;
		j++;
	}
	i = index + 1;
	while (i < old_count)
	{
		new_argv[pos] = ft_strdup(argv[i]);
		pos++;
		i++;
	}
	new_argv[pos] = NULL;
	i = 0;
	while (i < old_count)
	{
		free(argv[i]);
		i++;
	}
	free(argv);
	return (new_argv);
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


char	*expand_string_fixed(const char *str, t_shell *shell,
				int *is_fully_unquoted)
{
	char	*result;
	char	*tmp;
	int		i;
	int		start;
	int		fully_unquoted;
	int		only_quotes;
	char	*temp2;

	only_quotes = 1;
	i = 0;
	while (str[i])
	{
		if (str[i] != '\'' && str[i] != '"' && str[i] != ' ')
		{
			only_quotes = 0;
			break ;
		}
		i++;
	}
	if (only_quotes)
	{

		temp2 = malloc(ft_strlen(str) + 1);
		if (!temp2)
			return (NULL);
		i = 0;
		start = 0;
		while (str[i])
		{
			if (str[i] != '\'' && str[i] != '"')
			{
				temp2[start] = str[i];
				start++;
			}
			i++;
		}
		temp2[start] = '\0';
		return (ft_strdup(temp2));
	}
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	fully_unquoted = 1;
	while (str[i])
	{
		if (str[i] == '\'')
		{
			fully_unquoted = 0;
			i++; 
			start = i;
			while (str[i] && str[i] != '\'')
				i++;
			tmp = ft_substr(str, start, i - start);
			if (!is_all_spaces(tmp))
			{
				char	*comp = compress_spaces(tmp);
				free(tmp);
				tmp = comp;
			}
			result = ft_strjoin(result, tmp);
			free(tmp);
			if (str[i] == '\'')
				i++; 
		}
		else if (str[i] == '"')
		{
			fully_unquoted = 0;
			i++; 
			start = i;
			while (str[i] && str[i] != '"')
				i++;
			tmp = ft_substr(str, start, i - start);
			{
				char	*expanded_segment;
				int		j;

				expanded_segment = ft_strdup("");
				j = 0;
				while (tmp[j])
				{
					if (tmp[j] == '$')
					{
						j++;
						expanded_segment = ft_strjoin(expanded_segment,
							expand_variable(tmp, &j, shell));
					}
					else
					{
						char	c[2];

						c[0] = tmp[j];
						c[1] = '\0';
						j++;
						expanded_segment = ft_strjoin(expanded_segment, c);
					}
				}
				if (!is_all_spaces(expanded_segment))
				{
					char	*temp_compressed = compress_spaces(expanded_segment);
					free(expanded_segment);
					expanded_segment = temp_compressed;
				}
				result = ft_strjoin(result, expanded_segment);
				free(expanded_segment);
			}
			free(tmp);
			if (str[i] == '"')
				i++;
		}
		else
		{
			start = i;
			while (str[i] && str[i] != '\'' && str[i] != '"')
				i++;
			tmp = ft_substr(str, start, i - start);
			{
				char	*expanded_segment;
				int		j;

				expanded_segment = ft_strdup("");
				j = 0;
				while (tmp[j])
				{
					if (tmp[j] == '$')
					{
						j++;
						expanded_segment = ft_strjoin(expanded_segment,
							expand_variable(tmp, &j, shell));
					}
					else
					{
						char	c[2];

						c[0] = tmp[j];
						c[1] = '\0';
						j++;
						expanded_segment = ft_strjoin(expanded_segment, c);
					}
				}
				result = ft_strjoin(result, expanded_segment);
				free(expanded_segment);
			}
			free(tmp);
		}
	}
	*is_fully_unquoted = fully_unquoted;
	if (fully_unquoted)
	{
		tmp = ft_strtrim_spaces(result);
		free(result);
		result = compress_spaces(tmp);
		free(tmp);
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
	return (new_input);
}

int	expander(char ***argv_ptr, t_shell *shell)
{
	char	**argv;
	char	*expanded;
	char	*tmp;
	char	*old_arg;
	int		i;
	int		fully_unquoted;
	char	**split_tokens;

	argv = *argv_ptr;
	i = 0;
	while (argv[i])
	{
		if ((argv[i][0] == '"' || argv[i][0] == '\'') &&
    argv[i][1] == argv[i][0])
{
    int j = 0;
    int has_alpha = 0;

    while (argv[i][j])
    {
        if (ft_isalpha(argv[i][j]))
        {
            has_alpha = 1;
            break;
        }
        j++;
    }

    if (!has_alpha)
    {
        i++;
        continue;
    }
}

		if (i == 0 && (argv[i][0] == '"' || argv[i][0] == '\''))
		{
			tmp = ft_substr(argv[i], 1, ft_strlen(argv[i]) - 2);
			if (is_all_spaces(tmp))
			{
				free(argv[i]);
				argv[i] = tmp;
				i++;
				continue ;
			}
			free(tmp);
		}
		if (ft_strisspace(argv[i]))
		{
			i++;
			continue ;
		}
		old_arg = argv[i];
		expanded = expand_string_fixed(old_arg, shell, &fully_unquoted);
		if (!expanded || ft_strisspace(expanded))
		{
			free(expanded);
			remove_arg(&argv, i);
			continue ;
		}
		if (fully_unquoted)
		{
			split_tokens = ft_split(expanded, ' ');
			free(expanded);
			if (!split_tokens || !split_tokens[0])
			{
				if (split_tokens)
					free(split_tokens);
				remove_arg(&argv, i);
				continue ;
			}
			if (ft_tablen(split_tokens) > 1)
			{
				argv = replace_token_with_tokens(argv, i, split_tokens);
				free(split_tokens);
				continue ;
			}
			else
			{
				free(argv[i]);
				argv[i] = ft_strdup(split_tokens[0]);
				free(split_tokens);
			}
		}
		else
		{
			tmp = preprocess_input_test(expanded);
			free(expanded);
			free(argv[i]);
			argv[i] = tmp;
		}
		i++;
	}
	*argv_ptr = argv;
	return (i);
}

void	expander_test(char **argv, t_shell *shell)
{
	char	*expanded;
	char	*tmp;
	int		fully_unquoted;
	char	**split_tokens;

	expanded = expand_string_fixed(*argv, shell, &fully_unquoted);
	free(*argv);
	if (fully_unquoted)
	{
		split_tokens = ft_split(expanded, ' ');
		free(expanded);
		if (split_tokens && split_tokens[0])
			*argv = ft_strdup(split_tokens[0]);
		else
			*argv = ft_strdup("");
	}
	else
	{
		tmp = preprocess_input_test(expanded);
		free(expanded);
		*argv = tmp;
	}
}
