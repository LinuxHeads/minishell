/* ************************************************************************** */
/*                                                                            */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_enclosed_quotes(const char *str);

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
	if (ft_isdigit(str[start]))
	{
		var_name = ft_substr(str, start, *index - start);
		env_val = ft_getenv(var_name, shell->env_list);
		free(var_name);
		if (!env_val)
			env_val = "";
		expanded = ft_strdup(env_val);
		(*index)++;
		return (expanded);
	}
	else
	{
		while (str[*index] && (ft_isalnum(str[*index]) || str[*index] == '_'))
			(*index)++;
	}
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

char	*remove_outer_closed_quotes(const char *s)
{
	size_t	len;
	char	*result;
	size_t	i;
	size_t	j;
	char	quote;
	size_t	k;
	int		foundClosing;

	if (!s)
		return (NULL);
	len = strlen(s);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (i < len)
	{
		if (s[i] == '\'' || s[i] == '"')
		{
			quote = s[i];
			k = i + 1;
			foundClosing = 0;
			while (k < len)
			{
				if (s[k] == quote)
				{
					foundClosing = 1;
					break ;
				}
				k++;
			}
			if (foundClosing)
			{
				i++;
				while (i < len && s[i] != quote)
					result[j++] = s[i++];
				if (i < len && s[i] == quote)
					i++;
			}
			else
			{
				result[j++] = s[i++];
			}
		}
		else
		{
			result[j++] = s[i++];
		}
	}
	result[j] = '\0';
	return (result);
}

char	*expand_string(const char *str, t_shell *shell)
{
	char	*result;
	char	*var_value;
	int		i;
	int		in_sq;
	int		in_dq;
	char	*tmp;
	int		encl;
	char	*trim;
	char	c[2];

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
				encl = count_enclosed_quotes(var_value);
				if (encl > 1)
				{
					char *temp = remove_outer_closed_quotes(var_value);
					free(var_value);
					var_value = temp;
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
			c[0] = str[i];
			c[1] = '\0';
			encl = count_enclosed_quotes(result);
			if (encl == 2)
			{
				char *temp = remove_outer_closed_quotes(result);
				free(result);
				result = temp;
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

static int	ft_tablen(char **tab_1)
{
	int	i;

	i = 0;
	while (tab_1 && tab_1[i])
		i++;
	return (i);
}

static char	**replace_token_with_tokens(char **argv, int index, char **new_tokens)
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
		new_argv[pos++] = ft_strdup(argv[i++]);
	}
	j = 0;
	while (j < new_count)
	{
		new_argv[pos++] = ft_strdup(new_tokens[j++]);
	}
	i = index + 1;
	while (i < old_count)
	{
		new_argv[pos++] = ft_strdup(argv[i++]);
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

int	count_enclosed_quotes(const char *str)
{
	int			count;
	int			len;
	const char	*start;
	const char	*end;

	count = 0;
	len = ft_strlen(str);
	start = str;
	end = str + len - 1;
	while (len >= 2 &&
			((*start == '\'' && *end == '\'') || (*start == '"' && *end == '"')))
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

int	expander(t_shell **shell)  /* leaks fixed in this function */
{
	char	**argv;
	char	*expanded;
	char	*tmp;
	char	*old_arg;
	int		i;
	int		flag;
	char	*c;
	char	*no_closed_quotes;
	int		encl;
	char	**split_tokens;
	int		k;

	argv = (*shell)->argv;
	i = 0;
	flag = 1;
	while (argv[i])
	{
		old_arg = argv[i];
		flag = 1;
		if (check_single_qoutes(argv[i]) || check_double_qoutes(argv[i]))
		{
			c = malloc(3 * sizeof(char));
			if (!c)
				return (-1);
			c[0] = argv[i][0];
			c[1] = argv[i][1];
			c[2] = '\0';
			free(argv[i]);            /* Free previous allocation */
			argv[i] = c;
			i++;
			continue ;
		}
		if (!ft_strchr(old_arg, '$'))
		{
			no_closed_quotes = remove_outer_closed_quotes(old_arg);
			if (no_closed_quotes)
			{
				free(argv[i]);
				argv[i] = no_closed_quotes;
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
		flag = 0;
		encl = count_enclosed_quotes(old_arg);
		expanded = expand_string(old_arg, *shell);
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
		if (!encl && ft_strchr(expanded, ' '))
		{
			split_tokens = ft_splitter(expanded, ' ');
			free(expanded);
			if (!split_tokens || !split_tokens[0])
			{
				if (split_tokens)
				{
					k = 0;
					while (split_tokens[k])
						free(split_tokens[k++]);
					free(split_tokens);
				}
				remove_arg(&argv, i);
				continue ;
			}
			if (ft_tablen(split_tokens) > 1)
			{
				argv = replace_token_with_tokens(argv, i, split_tokens);
				k = 0;
				while (split_tokens[k])
					free(split_tokens[k++]);
				free(split_tokens);
				continue ;
			}
			else
			{
				free(argv[i]);
				argv[i] = ft_strdup(split_tokens[0]);
				k = 0;
				while (split_tokens[k])
					free(split_tokens[k++]);
				free(split_tokens);
			}
		}
		else
		{
			if (encl != 2 && encl != 0 && (!flag && encl != 1))
			{
				no_closed_quotes = remove_outer_closed_quotes(expanded);
				free(argv[i]);
				argv[i] = no_closed_quotes;
			}
			else
			{
				free(argv[i]);
				argv[i] = expanded;
			}
		}
		i++;
	}
	(*shell)->argv = argv;
	return (i);
}

void	expander_test(char **arg, t_shell *shell)
{
	char	*old_arg;
	char	*expanded;
	char	*tmp;
	int		flag;
	char	*c;
	char	*no_closed_quotes;
	int		encl;
	char	**split_tokens;

	flag = 1;
	if (!arg || !(*arg))
		return ;
	old_arg = *arg;
	if (check_single_qoutes(old_arg) || check_double_qoutes(old_arg))
	{
		c = malloc(3 * sizeof(char));
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
		no_closed_quotes = remove_outer_closed_quotes(old_arg);
		if (no_closed_quotes)
		{
			free(old_arg);
			old_arg = no_closed_quotes;
			*arg = old_arg;
		}
		tmp = preprocess_input_test(old_arg);
		if (tmp)
		{
			free(old_arg);
			*arg = tmp;
		}
		return ;
	}
	flag = 0;
	encl = count_enclosed_quotes(old_arg);
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
	if (!encl && ft_strchr(expanded, ' '))
	{
		split_tokens = ft_splitter(expanded, ' ');
		free(expanded);
		if (!split_tokens || !split_tokens[0])
		{
			if (split_tokens)
			{
				for (int k = 0; split_tokens[k]; k++)
					free(split_tokens[k]);
				free(split_tokens);
			}
			free(*arg);
			*arg = NULL;
			return ;
		}
		if (ft_tablen(split_tokens) > 1)
		{
			free(*arg);
			*arg = ft_strdup(split_tokens[0]);
		}
		else
		{
			free(*arg);
			*arg = ft_strdup(split_tokens[0]);
		}
		for (int k = 0; split_tokens[k]; k++)
			free(split_tokens[k]);
		free(split_tokens);
		return ;
	}
	else
	{
		if (encl != 2 && encl != 0 && (!flag && encl != 1))
		{
			no_closed_quotes = remove_outer_closed_quotes(expanded);
			free(*arg);
			*arg = no_closed_quotes;
		}
		else
		{
			free(*arg);
			*arg = expanded;
		}
		return ;
	}
}

// test leaks for expander_test function*
// int main()
// {
// 	t_shell	*shell;
// 	char	**argv;
// 	int		i;

// 	shell = malloc(sizeof(t_shell));
// 	if (!shell)
// 		return (1);
// 	shell->env_list = NULL;
// 	shell->envp = NULL;
// 	shell->exit_status = 0;
// 	argv = malloc(sizeof(char *) * 4);
// 	if (!argv)
// 	{
// 		free(shell);
// 		return (1);
// 	}
// 	argv[0] = ft_strdup("echo");
// 	argv[1] = ft_strdup("hello");
// 	argv[2] = ft_strdup("world");
// 	argv[3] = NULL;
// 	i = 0;
// 	while (argv[i])
// 	{
// 		expander_test(&argv[i], shell);
// 		i++;
// 	}
// 	i = 0;
// 	while (argv[i])
// 	{
// 		printf("%s\n", argv[i]);
// 		i++;
// 	}
// 	i = 0;
// 	while (argv[i])
// 	{
// 		free(argv[i]);
// 		i++;
// 	}
// 	free(argv);
// 	free(shell);
// 	return (0);
// }
