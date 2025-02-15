/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_arg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 17:47:31 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/15 17:47:58 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


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
			free_str_array(split_tokens);
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
	free_str_array(split_tokens);
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
		finalize_env_variable_token(expanded, arg);
	else
		finalize_expansion(expanded, flag, encl, arg);
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
