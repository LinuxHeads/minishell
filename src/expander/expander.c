/* ************************************************************************** */
/*                                                                            */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


int ft_strisspace(const char *str)
{
	int i;

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


char *remove_outer_closed_quotes(const char *s)
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
    if ((s[0] == '\'' || s[0] == '"') && s[len - 1] == s[0])
    {
        char quote = s[0];
        size_t k = 1;
        while (k < len - 1)
        {
            if (s[k] == quote)
                break;
            k++;
        }
        if (k == len - 1)
        {
            i = 1;
            len--; 
        }
    }

    while (i < len)
        res[j++] = s[i++];
    
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
	tmp = new_input;
	new_input = ft_str_replace(tmp, " |", "|");
	free(tmp);
	tmp = new_input;
	new_input = ft_str_replace(tmp, "| ", "|");
	free(tmp);

	return (new_input);
}


static int	ft_tablen(char **tab)
{
	int	i = 0;
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

int count_enclosed_quotes(const char *str)
{
    int count = 0;
    int len = ft_strlen(str);
    const char *start = str;
    const char *end = str + len - 1;

    while (len >= 2 && 
          (( *start == '\'' && *end == '\'') || (*start == '"' && *end == '"' )))
    {
        count++;
        start++;
        end--;
        len -= 2;
    }
    return count;
}


int	expander(char ***argv_ptr, t_shell *shell)
{
	char	**argv = *argv_ptr;
	char	*expanded;
	char	*tmp;
	char	*old_arg;
	int		i = 0;

	while (argv[i])
	{
		old_arg = argv[i];
		if (!ft_strchr(old_arg, '$'))
		{
			char *no_closed_quotes = remove_outer_closed_quotes(old_arg);
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
			continue;
		}

		// int had_quote = (ft_strchr(old_arg, '\'') || ft_strchr(old_arg, '"')) ? 1 : 0;
        // int had_quote_another = (old_arg[1]=='\'' || old_arg[1]=='\"' ) ? 1 : 0;
        int encl = count_enclosed_quotes(old_arg);
        // printf("HAD ? %d\n",had_quote);
		expanded = expand_string(old_arg, shell);
		if (!expanded || ft_strisspace(expanded))
		{
			free(expanded);
			remove_arg(&argv, i);
			continue;
		}
		tmp = preprocess_input_test(expanded);
		free(expanded);
		if (!tmp)
		{
			remove_arg(&argv, i);
			continue;
		}
		expanded = tmp;

           //int j=0;
		if (!encl && ft_strchr(expanded, ' '))
		{
            char *no_closed_quotes = remove_outer_closed_quotes(expanded);
			char **split_tokens = ft_split(no_closed_quotes, ' ');
            // while(split_tokens[j])
            // {
            //     if(split_tokens[j][0]=='\"' || split_tokens[j][0]== '\'')
            //     {
            //         remove_arg(&split_tokens, j);
            //     }
            //     // printf(" split_tokens[j] old %s\n", split_tokens[j] );
            //     // split_tokens[j] = remove_outer_closed_quotes(split_tokens[j]);
            //     // printf(" split_tokens[j] new %s\n", split_tokens[j] );
            //     j++;
            // }
			free(expanded);
			if (!split_tokens || !split_tokens[0])
			{
				if (split_tokens)
				{
					int k = 0;
					while (split_tokens[k])
						free(split_tokens[k++]);
					free(split_tokens);
				}
				remove_arg(&argv, i);
				continue;
			}
			if (ft_tablen(split_tokens) > 1)
			{
				argv = replace_token_with_tokens(argv, i, split_tokens);
			
				int k = 0;
				while (split_tokens[k])
					free(split_tokens[k++]);
				free(split_tokens);

				continue;
			}
			else
			{
				free(argv[i]);
				argv[i] = ft_strdup(split_tokens[0]);
				int k = 0;
				while (split_tokens[k])
					free(split_tokens[k++]);
				free(split_tokens);
			}
		}
		else
		{
            if(encl!=2)
            { 
                char *no_closed_quotes = remove_outer_closed_quotes(expanded);
			    free(argv[i]);
			    argv[i] = no_closed_quotes;}
                else{
                    argv[i]=expanded;
                }
		}
		i++;
	}
	*argv_ptr = argv;
	return (1);
}


void	expander_test(char **argv, t_shell *shell)
{
	char	*expanded;
	size_t	len;

	if (!ft_strchr(*argv, '$'))
	{
		char *no_closed_quotes = remove_outer_closed_quotes(*argv);
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
		char *no_closed_quotes = remove_outer_closed_quotes(*argv);
		if (no_closed_quotes)
		{
			free(*argv);
			*argv = no_closed_quotes;
		}
	}
}
