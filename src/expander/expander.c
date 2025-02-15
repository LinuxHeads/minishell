/* ************************************************************************** */
/*                                                                            */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_surrounding_quotes(const char *str);

int	is_whitespace(const char *str)
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

static char	*trim_spaces(const char *s)
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

void	remove_extra_spaces(char *new_str, const char *str, int *j)
{	
	int		i;
	int		in_space;

	i = 0;
	in_space = 0;
	while (str[i])
	{
		if (str[i] == ' ')
		{
			if (!in_space)
			{
				new_str[(*j)++] = ' ';
				in_space = 1;
			}
		}
		else
		{
			new_str[(*j)++] = str[i];
			in_space = 0;
		}
		i++;
	}
}

char	*compress_whitespace(const char *str)
{
	char	*new_str;
	int		j;

	if (!str)
		return (NULL);
	j = 0;
	if (!str)
		return (NULL);
	new_str = malloc(ft_strlen(str) + 1);
	if (!new_str)
		return (NULL);
	remove_extra_spaces(new_str, str, &j);
	new_str[j] = '\0';
	return (new_str);
}

static void	remove_argument(char ***argv_ptr, int index)
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

char	*expand_env_variable(const char *str, int *index, t_shell *shell)
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
		if (!var_name)
			return (NULL);
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

char	*strip_outers_quotes(const char *s)
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

char	*expand_env_string(const char *str, t_shell *shell)
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
			var_value = expand_env_variable(str, &i, shell);
			if (var_value)
			{
				encl = count_surrounding_quotes(var_value);
				if (encl > 1)
				{
					char *temp = strip_outers_quotes(var_value);
					free(var_value);
					var_value = temp;
				}
				if (!in_dq)
				{
					trim = trim_spaces(var_value);
					free(var_value);
					if (!trim)
					{
						free(result);
						return (NULL);
					}
					if (is_whitespace(trim))
					{
						free(trim);
						continue ;
					}
					var_value = compress_whitespace(trim);
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
			encl = count_surrounding_quotes(result);
			if (encl == 2)
			{
				char *temp = strip_outers_quotes(result);
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

char	*cleanup_input(char *input)
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

static int	array_length(char **tab_1)
{
	int	i;

	i = 0;
	while (tab_1 && tab_1[i])
		i++;
	return (i);
}

static char	**replace_token_in_array(char **argv, int index, char **new_tokens)
{
	int		old_count;
	int		new_count;
	int		total;
	char	**new_argv;
	int		i;
	int		j;
	int		pos;

	old_count = array_length(argv);
	new_count = array_length(new_tokens);
	total = old_count - 1 + new_count;
	new_argv = malloc(sizeof(char *) * (total + 1));
	if (!new_argv)
		return (NULL);
	pos = 0;
	i = 0;
	while (i < index)
	{
		new_argv[pos++] = ft_strdup(argv[i++]);
		if (!new_argv[pos - 1])
		{
			while (pos > 0)
				free(new_argv[--pos]);
			free(new_argv);
			return (NULL);
		}
	}
	j = 0;
	while (j < new_count)
	{
		new_argv[pos++] = ft_strdup(new_tokens[j++]);
		if (!new_argv[pos - 1])
		{
			while (pos > 0)
				free(new_argv[--pos]);
			free(new_argv);
			return (NULL);
		}
	}
	i = index + 1;
	while (i < old_count)
	{
		new_argv[pos++] = ft_strdup(argv[i++]);
		if (!new_argv[pos - 1])
		{
			while (pos > 0)
				free(new_argv[--pos]);
			free(new_argv);
			return (NULL);
		}
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

int	count_surrounding_quotes(const char *str)
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

int	is_single_quote_token(char *s)
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

int	is_double_quote_token(char *s)
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
		if (is_single_quote_token(argv[i]) || is_double_quote_token(argv[i]))
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
			if (ft_strcmp(old_arg, "~") == 0)
				old_arg = ft_getenv("HOME", (*shell)->env_list);
			no_closed_quotes = strip_outers_quotes(old_arg);
			if (no_closed_quotes)
			{
				free(argv[i]);
				argv[i] = no_closed_quotes;
			}
			tmp = cleanup_input(argv[i]);
			if (tmp)
			{
				free(argv[i]);
				argv[i] = tmp;
			}
			i++;
			continue ;
		}
		flag = 0;
		encl = count_surrounding_quotes(old_arg);
		expanded = expand_env_string(old_arg, *shell);
		if (!expanded || is_whitespace(expanded))
		{
			free(expanded);
			remove_argument(&argv, i);
			continue ;
		}
		tmp = cleanup_input(expanded);
		free(expanded);
		if (!tmp)
		{
			remove_argument(&argv, i);
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
				remove_argument(&argv, i);
				continue ;
			}
			if (array_length(split_tokens) > 1)
			{
				argv = replace_token_in_array(argv, i, split_tokens);
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
				no_closed_quotes = strip_outers_quotes(expanded);
				free(argv[i]);
				free(expanded);
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

static int check_for_sign(char *old_arg, char **arg)
{
	char	*no_closed_quotes;
	char	*tmp;

	if (!ft_strchr(old_arg, '$'))
	{
		no_closed_quotes = strip_outers_quotes(old_arg);
		if (no_closed_quotes)
		{
			free(old_arg);
			old_arg = no_closed_quotes;
			*arg = old_arg;
		}
		tmp = cleanup_input(old_arg);
		if (tmp)
		{
			free(old_arg);
			*arg = tmp;
		}
		return (1);
	}
	return (0);
}

static int check_for_quotes(char *old_arg, char **arg)
{
	char	*c;

	if (is_single_quote_token(old_arg) || is_double_quote_token(old_arg))
	{
		c = malloc(3 * sizeof(char));
		if (!c)
			return (2);
		c[0] = old_arg[0];
		c[1] = old_arg[1];
		c[2] = '\0';
		free(old_arg);
		*arg = c;
		return (1);
	}
	return (0);
}

static int	prepare_expanded_token(char **expanded, t_shell *shell, char *old_arg, char **arg)
{
	char	*tmp;

	*expanded = expand_env_string(old_arg, shell);
	if (!*expanded || is_whitespace(*expanded))
	{
		free(*expanded);
		free(*arg);
		*arg = NULL;
		return (0);
	}
	tmp = cleanup_input(*expanded);
	free(*expanded);
	if (!tmp)
	{
		free(*arg);
		*arg = NULL;
		return (0);
	}
	*expanded = tmp;
	return (1);
}

static void finalize_env_variable_token(char *expanded, char **arg)
{
	char	**split_tokens;

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
	if (array_length(split_tokens) > 1)
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

static void finalize_expansion(char	*expanded, int	flag, int	encl, char **arg)
{
	char	*no_closed_quotes;

	if (encl != 2 && encl != 0 && (!flag && encl != 1))
	{
		no_closed_quotes = strip_outers_quotes(expanded);
		free(*arg);
		*arg = no_closed_quotes;
	}
	else
	{
		free(*arg);
		*arg = expanded;
	}
}

void	expand_single_argument(char **arg, t_shell *shell)
{
	char	*old_arg;
	char	*expanded;
	int		flag;
	int		encl;

	flag = 1;
	if (!arg || !(*arg))
		return ;
	old_arg = *arg;
	if (check_for_quotes(old_arg, arg))
		return ;
	if (check_for_sign(old_arg, arg))
		return ;
	flag = 0;
	encl = count_surrounding_quotes(old_arg);
	if (!prepare_expanded_token(&expanded, shell, old_arg, arg))
		return ;
	if (!encl && ft_strchr(expanded, ' '))
	{
		finalize_env_variable_token(expanded, arg);
		return ;
	}
	else
	{
		finalize_expansion(expanded, flag, encl, arg);
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
