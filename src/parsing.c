/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yourlogin <yourlogin@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 01:23:07 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/02 15:00:00 by yourlogin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <string.h>

# include "../libft/libft.h"

static char	**ft_free(char **str, int i)
{
	int	j = 0;

	while (j < i)
	{
		free(str[j]);
		j++;
	}
	free(str);
	return (NULL);
}


static size_t	get_sep(const char *s, char c)
{
	size_t	count = 0;

	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (!*s)
			break ;
		count++;
		while (*s && *s != c)
		{
			if (*s == '"' || *s == '\'')
			{
				char quote = *s++;
				while (*s && *s != quote)
					s++;
				if (*s)
					s++;
			}
			else
				s++;
		}
	}
	return (count);
}

static char	*extract_word(const char *start, const char *end)
{
	size_t	len = 0;
	const char *s = start;

	// if ((*start == '"' || *start == '\'') && *(end - 1) == *start)
	// {
	// 	start++;
	// 	end--;
	// }

	len = end - start;

	char	*word = malloc(len + 1);
	if (!word)
		return (NULL);
	size_t i = 0;
	while ( i < len)
	{
		word[i] = start[i];
		i++;
	}
	word[len] = '\0';

	return (word);
}

char	**ft_split(const char *s, char c)
{
	char	**result;
	size_t	count, index = 0;
	const char	*start;

	if (!s)
		return (NULL);
	count = get_sep(s, c);
	result = malloc((count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (!*s)
			break ;
		start = s;
		while (*s && *s != c)
		{
			if (*s == '"' || *s == '\'')
			{
				char quote = *s++;
				while (*s && *s != quote)
					s++;
				if (*s)
					s++;
			}
			else
				s++;
		}
		result[index] = extract_word(start, s);
		if (!result[index])
			return (ft_free(result, index));
		index++;
	}
	result[index] = NULL;
	return (result);
}



static void	free_str_array(char **arr)
{
	int	i = 0;

	if (!arr)
		return;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

int	count_words(char *str, char sep)
{
	int		count;
	char	**tokens;

	count = 0;
	tokens = ft_split(str, sep);
	if (!tokens)
		return (0);
	while (tokens[count])
		count++;
	free_str_array(tokens);
	return (count);
}


typedef enum e_token_type
{
	COMMAND,
	ARGUMENT,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	REDIRECT_APPEND,
	HEREDOC,
	HEREDOC_DELI,
	ENV_VAR,
	DOLLAR_SIGN,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
	INPUT_FILE,
	OUTPUT_FILE
}	t_token_type;

typedef struct s_next_token
{
	int	command;
	int	argument;
	int	env_var;
	int here_doc;
	int	input_file;
	int	output_file;
}	t_next_token;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
}	t_token;

typedef struct s_command
{
	t_token	**tokens;
	int		token_count;
}	t_command;

typedef struct s_shell
{
	t_command	**commands;
	int			command_count;
}	t_shell;


t_token_type	identify_token_type(char *token, t_next_token *decide)
{
	size_t	len = ft_strlen(token);
	
	if (ft_strcmp(token, "|") == 0)
	{
		decide->command = 1;
		return (PIPE);
	}
	if (ft_strcmp(token, ">>") == 0)
	{
		decide->output_file = 1;
		return (REDIRECT_APPEND);
	}
	if (ft_strcmp(token, ">") == 0)
	{
		decide->output_file = 1;
		return (REDIRECT_OUT);
	}
	if (ft_strcmp(token, "<<") == 0)
	{
		decide->here_doc = 1;
		return (HEREDOC);
	}
	if (decide->here_doc == 1)
	{
		decide->here_doc = 0;
		return (HEREDOC_DELI);
	}
	if (ft_strcmp(token, "<") == 0)
	{
		decide->input_file = 1;
		return (REDIRECT_IN);
	}
	if (token[0] == '$')
	{
		if (len == 1)
			return (DOLLAR_SIGN);
		return (ENV_VAR);
	}
	if (token[0] == '\'' && token[len - 1] == '\'')
		return (SINGLE_QUOTE);
	if (token[0] == '"' && token[len - 1] == '"')
		return (DOUBLE_QUOTE);
	if (decide->input_file == 1)
	{
		decide->input_file = 0;
		return (INPUT_FILE);
	}
	if (decide->output_file == 1)
	{
		decide->output_file = 0;
		return (OUTPUT_FILE);
	}
	if (decide->command == 1)
	{
		decide->command = 0;
		return (COMMAND);
	}
	return (ARGUMENT);
}


t_shell	*allocate_shell_commands(int num_commands, char **shell_command)
{
	t_shell			*shell;
	t_next_token	decide;
	int				i, j;
	char			**token_str;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->commands = malloc(sizeof(t_command *) * (num_commands + 1));
	if (!shell->commands)
	{
		free(shell);
		return (NULL);
	}
	shell->command_count = num_commands;
	i = 0;
	while (i < num_commands)
	{
		decide = (t_next_token){1, 0, 0, 0, 0};
		shell->commands[i] = malloc(sizeof(t_command));
		if (!shell->commands[i])
			return (NULL);
		shell->commands[i]->token_count = count_words(shell_command[i], ' ');
		shell->commands[i]->tokens = malloc(sizeof(t_token *) *
				(shell->commands[i]->token_count + 1));
		if (!shell->commands[i]->tokens)
		{
			free(shell->commands[i]);
			return (NULL);
		}
		token_str = ft_split(shell_command[i], ' ');
		if (!token_str)
			return (NULL);
		j = 0;
		while (token_str[j])
		{
			shell->commands[i]->tokens[j] = malloc(sizeof(t_token));
			if (!shell->commands[i]->tokens[j])
				return (NULL);
			shell->commands[i]->tokens[j]->value = ft_strdup(token_str[j]);
			if (!shell->commands[i]->tokens[j]->value)
				return (NULL);
			shell->commands[i]->tokens[j]->type =
				identify_token_type(token_str[j], &decide);
			j++;
		}
		shell->commands[i]->tokens[j] = NULL;
		free_str_array(token_str);
		i++;
	}
	shell->commands[i] = NULL;
	return (shell);
}


void	free_shell(t_shell *shell)
{
	int	i, j;

	if (!shell)
		return;
	i = 0;
	while (i < shell->command_count)
	{
		j = 0;
		while (j < shell->commands[i]->token_count)
		{
			free(shell->commands[i]->tokens[j]->value);
			free(shell->commands[i]->tokens[j]);
			j++;
		}
		free(shell->commands[i]->tokens);
		free(shell->commands[i]);
		i++;
	}
	free(shell->commands);
	free(shell);
}

const char	*get_token_type_name(t_token_type type)
{
	const char *names[] = {
		"COMMAND",
		"ARGUMENT",
		"PIPE",
		"REDIRECT_IN",
		"REDIRECT_OUT",
		"REDIRECT_APPEND",
		"HEREDOC",
		"HEREDOC_DELI",
		"ENV_VAR",
		"DOLLAR_SIGN",
		"SINGLE_QUOTE",
		"DOUBLE_QUOTE",
		"INPUT_FILE",
		"OUTPUT_FILE"
	};
	return (names[type]);
}

void	print_shell(t_shell *shell)
{
	int	i, j;

	printf("Parsed Shell Commands:\n");
	i = 0;
	while (i < shell->command_count)
	{
		printf("Command %d:\n", i + 1);
		j = 0;
		while (j < shell->commands[i]->token_count)
		{
			printf("  Token %d: %s (Type: %s)\n", j + 1,
				shell->commands[i]->tokens[j]->value,
				get_token_type_name(shell->commands[i]->tokens[j]->type));
			j++;
		}
		i++;
	}
}

char	*ft_str_replace(const char *str, const char *old, const char *new)
{
	char	*result;
	int		count = 0;
	int		old_len, new_len;
	int		i, j;
	const char *tmp = str;

	if (!str || !old || !new)
		return (NULL);
	old_len = ft_strlen(old);
	new_len = ft_strlen(new);
	while (*tmp)
	{
		if (ft_strncmp(tmp, old, old_len) == 0)
		{
			count++;
			tmp += old_len;
		}
		else
			tmp++;
	}
	result = malloc(ft_strlen(str) + count * (new_len - old_len) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (ft_strncmp(&str[i], old, old_len) == 0)
		{
			memcpy(&result[j], new, new_len);
			j += new_len;
			i += old_len;
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

char	*preprocess_input(char *input)
{
	char	*new_input;
	char	*tmp;

	if (!input)
		return (NULL);
	new_input = ft_str_replace(input, ">", " > ");
	if (!new_input)
		return (NULL);
	tmp = new_input;
	new_input = ft_str_replace(tmp, "<", " < ");
	free(tmp);
	if (!new_input)
		return (NULL);
	tmp = new_input;
	new_input = ft_str_replace(tmp, " >  > ", " >> ");
	free(tmp);
	if (!new_input)
		return (NULL);
	tmp = new_input;
	new_input = ft_str_replace(tmp, " <  < ", " << ");
	free(tmp);
	return (new_input);
}

int	main(void)
{
	t_shell	*shell;
	char	**commands;
	int		num_commands;
	char	*input;

	while (1)
	{
		input = readline("\033[1;32mminishell>\033[0m ");
		if (!input)
		{
			printf("\033[1;31mexit\033[0m\n");
			break ;
		}
		if (*input)
			add_history(input);
		input = preprocess_input(input);
		commands = ft_split(input, '|');
		num_commands = count_words(input, '|');
		shell = allocate_shell_commands(num_commands, commands);
		print_shell(shell);
		free_shell(shell);
		free_str_array(commands);
		free(input);
	}
	clear_history();
	return (0);
}