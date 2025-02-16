/* ************************************************************************** */
/*                                                                            */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	process_quote_segment(size_t *i, size_t *j, char *result, const char *s)
{
	int		foundClosing;
	char	quote;
	size_t	len;

	len = strlen(s);
	if (s[*i] == '\'' || s[*i] == '"')
	{
		quote = s[*i];
		foundClosing = has_closing(*i, quote, s, len);
		if (foundClosing)
		{
			(*i)++;
			while (*i < len && s[*i] != quote)
				result[(*j)++] = s[(*i)++];
			if (*i < len && s[*i] == quote)
				(*i)++;
		}
		else
			result[(*j)++] = s[(*i)++];
		return (1);
	}
	return (0);
}

char	*strip_outers_quotes(const char *s)
{
	size_t	len;
	char	*result;
	size_t	i;
	size_t	j;

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
		if (process_quote_segment(&i, &j, result, s))
			continue ;
		else
			result[j++] = s[i++];
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


static int	copy_tokens(char **dest, char **src, int *st_en_pos)
{

	while (st_en_pos[0] < st_en_pos[1])
	{
		dest[st_en_pos[2]] = ft_strdup(src[st_en_pos[0]]);
		if (!dest[st_en_pos[2]])
		{
			return (-1);
		}
		st_en_pos[2]++;
		st_en_pos[0]++;
	}
	return (st_en_pos[2]);
}

static void	free_old_argv(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

static int assemble_replacement_argv(int index, char **new_argv, char **argv, char **new_tokens)
{
	int 	pos;
	int		temp;
	int		count;	

	count = array_length(new_tokens);
	pos = copy_tokens(new_argv, argv,(int []){0, index, 0});
	if (pos == -1)
		return (-2);
	temp = pos;
	pos = copy_tokens(new_argv, new_tokens,(int []){0, count, pos});
	if (pos == -1)
	{
		free_tokens(new_argv, temp);
		return (0);
	}
	count = array_length(argv);
	pos = copy_tokens(new_argv, argv,(int []){index + 1, count, pos});
	if (pos == -1)
	{
		free_tokens(new_argv, temp);
		return (0);
	}
	return (pos);
}

static char	**replace_token_in_array(char **argv, int index, char **new_tokens)
{
	int		old_count;
	int		new_count;
	char	**new_argv;
	int		pos;

	old_count = array_length(argv);
	new_count = array_length(new_tokens);
	new_argv = malloc(sizeof(char *) * (old_count - 1 + new_count + 1));
	if (!new_argv)
		return (NULL);
	pos = assemble_replacement_argv(index, new_argv, argv, new_tokens);
	if (!pos || pos == -2)
	{
		if (pos == -2)
			free(new_argv);
		return (NULL);
	}
	free_old_argv(argv);
	new_argv[pos] = NULL;
	return (new_argv);
}

/*
** Allocates a new token that keeps only the first two characters.
*/
static char	*handle_quote_token(char *token)
{
	char	*c;

	c = malloc(3 * sizeof(char));
	if (!c)
		return (NULL);
	c[0] = token[0];
	c[1] = token[1];
	c[2] = '\0';
	return (c);
}

/*
** Processes a token that does NOT contain '$'.
** If the token equals "~", it gets replaced by the HOME env var.
** Then, outer quotes are stripped and the token is cleaned up.
*/
static int	process_no_env_token(char **token_ptr, t_shell *shell)
{
	char	*old_arg;
	char	*no_closed;
	char	*tmp;

	old_arg = *token_ptr;
	if (ft_strcmp(old_arg, "~") == 0)
		old_arg = ft_getenv("HOME", shell->env_list);
	no_closed = strip_outers_quotes(old_arg);
	if (no_closed)
	{
		free(*token_ptr);
		*token_ptr = no_closed;
	}
	tmp = cleanup_input(*token_ptr);
	if (tmp)
	{
		free(*token_ptr);
		*token_ptr = tmp;
	}
	return (0);
}

static int process_expanded_token_with_spaces(char ***argv_ptr, 	char	**argv, int i, char *expanded)
{
	char	**split_tokens;

	split_tokens = ft_splitter(expanded, ' ');
	free(expanded);
	if (!split_tokens)
		return (-1);
	if (!split_tokens || !split_tokens[0])
	{
		if (split_tokens)
			free_str_array(split_tokens);
		remove_argument(argv_ptr, i);
		return (0);
	}
	if (array_length(split_tokens) > 1)
	{
		*argv_ptr = replace_token_in_array(argv, i, split_tokens);
		if (!*argv_ptr)
		{
			free_str_array(split_tokens);
			return (-1);
		}
		free_str_array(split_tokens);
		return (0);
	}
	else
	{
		free(argv[i]);
		argv[i] = ft_strdup(split_tokens[0]);
		free_str_array(split_tokens);
		if (!argv[i])
			return (-1);
	}
	return (1);
}

static int finalize_expanded_token(int encl, char *expanded, char **argv, int i)
{
	char *no_closed;

	if (encl != 2 && encl != 0 && encl != 1)
	{
		no_closed = strip_outers_quotes(expanded);
		free(expanded);
		if (!no_closed)
			return (0);
		free(argv[i]);
		argv[i] = no_closed;
	}
	else
	{
		free(argv[i]);
		argv[i] = expanded;
	}
	return (1);
}

static char *expand_and_cleanup_token(char ***argv_ptr, char **argv, int i, t_shell *shell)
{
	char	*tmp;
	char	*expanded;

	expanded = expand_env_string(argv[i], shell);
	if (!expanded || is_whitespace(expanded))
	{
		free(expanded);
		remove_argument(argv_ptr, i);
		return (0);
	}
	tmp = cleanup_input(expanded);
	free(expanded);
	if (!tmp)
	{
		remove_argument(argv_ptr, i);
		return (0);
	}
	expanded = tmp;
	return (expanded);
}

/*
** Processes a token that contains '$'. It expands the env variables,
** cleans the result and, if needed, splits the token if it contains spaces.
** Depending on the outcome, the token at index i may be removed or updated.
**
** Returns 1 if the token remains (and i should be incremented) or 0 if the
** token was removed/replaced (and i should not be incremented).
*/
static int	process_env_token(char ***argv_ptr, t_shell *shell, int i)
{
	char	**argv;
	char	*expanded;
	int		encl;
	int		temp;

	argv = *argv_ptr;
	expanded = expand_and_cleanup_token(argv_ptr, argv, i, shell);
	if (!expanded)
		return (0);
	encl = count_surrounding_quotes(argv[i]);
	if (!encl && ft_strchr(expanded, ' '))
	{
		temp = process_expanded_token_with_spaces(argv_ptr, argv, i, expanded);
		if (temp == -1)
			return (-1);
		else if (temp == 0)
			return (0);
	}
	else
	{
		if (!finalize_expanded_token(encl, expanded, argv, i))
			return (-1);
	}
	return (1);
}

/*
** The expander function iterates over the argv tokens and processes each:
**
** - If the token is a simple quote token, it is reduced to its first two
**   characters.
** - If it contains no '$', it is cleaned-up (with "~" expanded to HOME).
** - Otherwise, environment variables are expanded, cleaned and if the
**   resulting token contains spaces, it may be split into multiple tokens.
**
** The function returns the number of tokens in the resulting argv.
*/
int	process_simple_token(char ***argv, int i, t_shell **shell)
{
	int		ret;
	char	*tmp;

	if (is_single_quote_token((*argv)[i]) || is_double_quote_token((*argv)[i]))
	{
		ret = 0;
		tmp = handle_quote_token((*argv)[i]);
		if (!tmp)
			return (-1);
		free((*argv)[i]);
		(*argv)[i] = tmp;
		return (1);
	}
	if (!ft_strchr((*argv)[i], '$'))
	{
		ret = process_no_env_token(&((*argv)[i]), *shell);
		if (ret < 0)
			return (-1);
		return (1);
	}
	return (0);
}


int	expander(t_shell **shell)
{
	char	**argv;
	int		i;
	int		ret;

	argv = (*shell)->argv;
	i = 0;
	while (argv[i])
	{
		ret = process_simple_token(&argv, i, shell);
		if (ret <= 0)
			if (ret < 0)
				return (-1);
		if (ret == 1)
			i++;
		if (ret == 1)
			continue ;
		ret = process_env_token(&argv, *shell, i);
		if (ret < 0)
			return (-1);
		if (ret == 1)
			i++;
	}
	(*shell)->argv = argv;
	return (i);
}
