/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 00:46:38 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/11 18:58:00 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/minishell.h"
#include <stdlib.h>
#include <string.h>

/* --- Forward declarations for internal (static) helper functions --- */
static char	*ft_strtrim_spaces(const char *s);
static void	remove_arg(char ***argv_ptr, int index);
static char	*expand_variable_digit(const char *str, int *index, t_shell *shell,
				int start);
static int	find_closing_quote(const char *s, int start, int len, char quote);
static int	process_quote(const char *s, int len, int i, char *res, int *j);
static char	*handle_dollar(const char *str, int *i, t_shell *shell, int in_dq,
				char *result);
static int	ft_tablen(char **tab);
static void	copy_tokens(char **dest, char **src, int start, int end, int *pos);
/* Marked as unused to silence the warning if it is not called anywhere */
static char	**replace_token_with_tokens(char **argv, int index,
				char **new_tokens) __attribute__((unused));
int			count_enclosed_quotes(const char *str);
/* --- Function definitions --- */

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
	end = (int)strlen(s) - 1;
	while (end >= start && s[end] == ' ')
		end--;
	if (end < start)
		return (strdup(""));
	trim = malloc(end - start + 2);
	if (!trim)
		return (NULL);
	i = 0;
	while (start <= end)
		trim[i++] = s[start++];
	trim[i] = '\0';
	return (trim);
}

static int	compress_spaces_loop(const char *str, char *new_str)
{
	int	i;
	int	j;
	int	in_space;

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
	return (j);
}

char	*compress_spaces(const char *str)
{
	char	*new_str;

	if (!str)
		return (NULL);
	new_str = malloc(strlen(str) + 1);
	if (!new_str)
		return (NULL);
	compress_spaces_loop(str, new_str);
	return (new_str);
}

static void	remove_arg(char ***argv_ptr, int index)
{
	char	**argv;
	int		i;

	argv = *argv_ptr;
	free(argv[index]);
	i = index;
	while (argv[i + 1])
	{
		argv[i] = argv[i + 1];
		i++;
	}
	argv[i] = NULL;
}

static char	*expand_variable_digit(const char *str, int *index, t_shell *shell,
		int start)
{
	char	*var_name;
	char	*env_val;
	char	*expanded;

	var_name = ft_substr(str, start, *index - start);
	env_val = ft_getenv(var_name, shell->env_list);
	free(var_name);
	if (!env_val)
		env_val = "";
	expanded = ft_strdup(env_val);
	(*index)++;
	return (expanded);
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
	if (ft_isdigit(str[start]))
		return (expand_variable_digit(str, index, shell, start));
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

static int	find_closing_quote(const char *s, int start, int len, char quote)
{
	while (start < len)
	{
		if (s[start] == quote)
			return (start);
		start++;
	}
	return (-1);
}

static int	process_quote(const char *s, int len, int i, char *res, int *j)
{
	int	close;

	close = find_closing_quote(s, i + 1, len, s[i]);
	if (close == -1)
	{
		res[(*j)++] = s[i++];
		return (i);
	}
	i++;
	while (i < close)
		res[(*j)++] = s[i++];
	return (close + 1);
}

char	*remove_outer_closed_quotes(const char *s)
{
	int		len;
	char	*res;
	int		i;
	int		j;

	if (!s)
		return (NULL);
	len = (int)strlen(s);
	res = malloc(len + 1);
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (i < len)
	{
		if ((s[i] == '\'' || s[i] == '"') && (find_closing_quote(s, i + 1, len,
				s[i]) != -1))
			i = process_quote(s, len, i, res, &j);
		else
		{
			res[j++] = s[i++];
		}
	}
	res[j] = '\0';
	return (res);
}

static char	*handle_dollar(const char *str, int *i, t_shell *shell, int in_dq,
		char *result)
{
	char	*var_value;
	char	*tmp;
	char	*temp;
	char	*trim;

	var_value = expand_variable(str, i, shell);
	if (var_value)
	{
		if (count_enclosed_quotes(var_value) > 1)
		{
			temp = var_value;
			var_value = remove_outer_closed_quotes(var_value);
			free(temp);
		}
		if (!in_dq)
		{
			trim = ft_strtrim_spaces(var_value);
			free(var_value);
			if (!trim)
			{
				free(result);
				return (NULL);
			}
			if (ft_strisspace(trim))
			{
				free(trim);
				return (result);
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
	return (result);
}

char	*expand_string(const char *str, t_shell *shell)
{
	char	*result;
	char	*tmp;
	int		i;
	int		in_sq;
	int		in_dq;
	char	c[2];
	char	*rtemp;

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
			result = handle_dollar(str, &i, shell, in_dq, result);
			if (!result)
				return (NULL);
			continue ;
		}
		else
		{
			c[0] = str[i];
			c[1] = '\0';
			if (count_enclosed_quotes(result) == 2)
			{
				rtemp = remove_outer_closed_quotes(result);
				free(result);
				result = rtemp;
			}
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
	tmp = ft_str_replace(new_input, " < ", "<");
	free(new_input);
	new_input = tmp;
	tmp = ft_str_replace(new_input, " >> ", ">>");
	free(new_input);
	new_input = tmp;
	tmp = ft_str_replace(new_input, " << ", "<<");
	free(new_input);
	new_input = tmp;
	tmp = ft_str_replace(new_input, " |", "|");
	free(new_input);
	new_input = tmp;
	tmp = ft_str_replace(new_input, "| ", "|");
	free(new_input);
	return (tmp);
}

static int	ft_tablen(char **tab)
{
	int	i;

	i = 0;
	while (tab && tab[i])
		i++;
	return (i);
}

static void	copy_tokens(char **dest, char **src, int start, int end, int *pos)
{
	int	i;

	i = start;
	while (i < end)
	{
		dest[*pos] = ft_strdup(src[i]);
		(*pos)++;
		i++;
	}
}

static char	**replace_token_with_tokens(char **argv, int index,
		char **new_tokens)
{
	int		old_count;
	int		new_count;
	int		total;
	char	**new_argv;
	int		pos;
	int		i;

	old_count = ft_tablen(argv);
	new_count = ft_tablen(new_tokens);
	total = old_count - 1 + new_count;
	new_argv = malloc(sizeof(char *) * (total + 1));
	if (!new_argv)
		return (NULL);
	pos = 0;
	copy_tokens(new_argv, argv, 0, index, &pos);
	copy_tokens(new_argv, new_tokens, 0, new_count, &pos);
	copy_tokens(new_argv, argv, index + 1, old_count, &pos);
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

int	count_enclosed_quotes(const char *str)
{
	int			count;
	int			len;
	const char	*start;
	const char	*end;

	count = 0;
	len = (int)strlen(str);
	start = str;
	end = str + len - 1;
	while (len >= 2 && (((*start == '\'' && *end == '\'') || (*start == '"'
					&& *end == '"'))))
	{
		count++;
		start++;
		end--;
		len -= 2;
	}
	return (count);
}

int	check_single_qoutes(char *s)
{
	int	i;

	i = 0;
	while (s[i] == '\'' && s[i])
	{
		i++;
		if (s[i] != '\'' && s[i])
			return (0);
	}
	return (i);
}

int	check_double_qoutes(char *s)
{
	int	i;

	i = 0;
	while (s[i] == '\"' && s[i])
	{
		i++;
		if (s[i] != '\"' && s[i])
			return (0);
	}
	return (i);
}

int	expander(char ***argv_ptr, t_shell *shell)
{
	char	**argv;
	char	*expanded;
	char	*tmp;
	char	*old_arg;
	int		i;
	char	*c;
	char	*nc;
	char	**split_tokens;
	int		k;

	argv = *argv_ptr;
	i = 0;
	while (argv[i])
	{
		old_arg = argv[i];
		if (check_single_qoutes(old_arg) || check_double_qoutes(old_arg))
		{
			c = malloc(3);
			if (!c)
				return (-1);
			c[0] = old_arg[0];
			c[1] = old_arg[1];
			c[2] = '\0';
			free(argv[i]);
			argv[i] = c;
			i++;
			continue ;
		}
		if (!ft_strchr(old_arg, '$'))
		{
			nc = remove_outer_closed_quotes(old_arg);
			if (nc)
			{
				free(argv[i]);
				argv[i] = nc;
			}
			tmp = preprocess_input_test(argv[i]);
			if (tmp)
			{
				free(argv[i]);
				argv[i] = tmp;
			}
			i++;
			continue ;
		}
		expanded = expand_string(old_arg, shell);
		if (!expanded || ft_strisspace(expanded))
		{
			free(expanded);
			remove_arg(&argv, i);
			continue ;
		}
		tmp = preprocess_input_test(expanded);
		free(expanded);
		if (!tmp)
		{
			remove_arg(&argv, i);
			continue ;
		}
		expanded = tmp;
		if (!count_enclosed_quotes(old_arg) && ft_strchr(expanded, ' '))
		{
			split_tokens = ft_splitter(expanded, ' ');
			free(expanded);
			if (!split_tokens || !split_tokens[0])
			{
				if (split_tokens)
				{
					k = 0;
					while (split_tokens[k])
					{
						free(split_tokens[k]);
						k++;
					}
					free(split_tokens);
				}
				remove_arg(&argv, i);
				continue ;
			}
			free(argv[i]);
			argv[i] = ft_strdup(split_tokens[0]);
			{
				k = 0;
				while (split_tokens[k])
				{
					free(split_tokens[k]);
					k++;
				}
				free(split_tokens);
			}
		}
		else
		{
			free(argv[i]);
			argv[i] = expanded;
		}
		i++;
	}
	*argv_ptr = argv;
	return (i);
}

void	expander_test(char **arg, t_shell *shell)
{
	char	*old_arg;
	char	*expanded;
	char	*tmp;
	char	*c;
	char	*nc;

	if (!arg || !(*arg))
		return ;
	old_arg = *arg;
	if (check_single_qoutes(old_arg) || check_double_qoutes(old_arg))
	{
		c = malloc(3);
		if (!c)
			return ;
		c[0] = old_arg[0];
		c[1] = old_arg[1];
		c[2] = '\0';
		free(old_arg);
		*arg = c;
		return ;
	}
	if (!ft_strchr(old_arg, '$'))
	{
		nc = remove_outer_closed_quotes(old_arg);
		if (nc)
		{
			free(old_arg);
			*arg = nc;
		}
		tmp = preprocess_input_test(*arg);
		if (tmp)
		{
			free(*arg);
			*arg = tmp;
		}
		return ;
	}
	expanded = expand_string(old_arg, shell);
	if (!expanded || ft_strisspace(expanded))
	{
		free(expanded);
		free(*arg);
		*arg = NULL;
		return ;
	}
	tmp = preprocess_input_test(expanded);
	free(expanded);
	if (!tmp)
	{
		free(*arg);
		*arg = NULL;
		return ;
	}
	expanded = tmp;
	free(*arg);
	*arg = expanded;
}
