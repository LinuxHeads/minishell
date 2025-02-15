/* ************************************************************************** */
/*                                                                            */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
					free_str_array(split_tokens);
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
				free_str_array(split_tokens);
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
