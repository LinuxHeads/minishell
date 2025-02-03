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

#include "../include/minishell.h"

static char	**ft_free(char **str, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		free(str[j]);
		j++;
	}
	free(str);
	return (NULL);
}

static void	skip_quote(const char **s)
{
	char	quote;

	quote = **s;
	(*s)++;
	while (**s && **s != quote)
		(*s)++;
	if (**s)
		(*s)++;
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
				skip_quote(&s);
			else
				s++;
		}
	}
	return (count);
}

static char	*extract_word(const char *start, const char *end)
{
	size_t	len;
	char	*word;
	size_t	i;

	len = end - start;
	word = malloc(len + 1);
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = start[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static char	*get_token(const char **s, char c)
{
	const char	*start;
	char		*word;

	while (**s && **s == c)
		(*s)++;
	if (!**s)
		return (NULL);
	start = *s;
	while (**s && **s != c)
	{
		if (**s == '"' || **s == '\'')
			skip_quote(s);
		else
			(*s)++;
	}
	word = extract_word(start, *s);
	return (word);
}

char	**ft_split(const char *s, char c)
{
	char		**result;
	size_t		count;
	size_t		index;
	const char	*ptr;

	if (!s)
		return (NULL);
	count = get_sep(s, c);
	result = malloc((count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	index = 0;
	ptr = s;
	while (1)
	{
		char *token = get_token(&ptr, c);
		if (!token)
			break ;
		result[index++] = token;
	}
	result[index] = NULL;
	return (result);
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

/* --- Helper to allocate a token --- */
t_token_type	identify_token_type(char *token, t_next_token *decide)
{
	size_t	len;

	len = ft_strlen(token);
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
static t_token	*allocate_token(char *str, t_next_token *decide)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(str);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->type = identify_token_type(str, decide);
	return (token);
}

/* --- Fills a command’s token array --- */

static int	fill_command_tokens(t_command *cmd, char **t_str,
				t_next_token *decide)
{
	int	j;

	j = 0;
	while (t_str[j])
	{
		cmd->tokens[j] = allocate_token(t_str[j], decide);
		if (!cmd->tokens[j])
			return (-1);
		j++;
	}
	cmd->tokens[j] = NULL;
	return (0);
}

/* --- Allocates a single command --- */

static t_command	*allocate_command(char *cmd_str)
{
	t_command	*cmd;
	t_next_token	decide;
	char		**t_str;

	decide = (t_next_token){1, 0, 0, 0, 0, 0};
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->token_count = count_words(cmd_str, ' ');
	cmd->tokens = malloc(sizeof(t_token *) * (cmd->token_count + 1));
	if (!cmd->tokens)
	{
		free(cmd);
		return (NULL);
	}
	t_str = ft_split(cmd_str, ' ');
	if (!t_str)
		return (NULL);
	if (fill_command_tokens(cmd, t_str, &decide) < 0)
		return (NULL);
	free_str_array(t_str);
	return (cmd);
}

/* --- Allocates all shell commands --- */

t_exec	*allocate_shell_commands(int num_commands, char **shell_command)
{
	t_exec	*shell;
	int		i;

	if (!(shell = malloc(sizeof(t_exec))))
		return (NULL);
	if (!(shell->commands = malloc(sizeof(t_command *) *
		(num_commands + 1))))
	{
		free(shell);
		return (NULL);
	}
	shell->command_count = num_commands;
	i = 0;
	while (i < num_commands)
	{
		shell->commands[i] = allocate_command(shell_command[i]);
		if (!shell->commands[i])
			return (NULL);
		i++;
	}
	shell->commands[i] = NULL;
	return (shell);
}

void	free_shell(t_exec *shell)
{
	int	i;
	int	j;

	if (!shell)
		return ;
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
	const char	*names[] = {
		"COMMAND", "ARGUMENT", "PIPE", "REDIRECT_IN", "REDIRECT_OUT",
		"REDIRECT_APPEND", "HEREDOC", "HEREDOC_DELI", "ENV_VAR",
		"DOLLAR_SIGN", "SINGLE_QUOTE", "DOUBLE_QUOTE",
		"INPUT_FILE", "OUTPUT_FILE"
	};

	return (names[type]);
}

void	print_shell(t_exec *shell)
{
	int	i;
	int	j;

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
	tmp = new_input;
	new_input = ft_str_replace(tmp, " >  > ", " >> ");
	free(tmp);
	tmp = new_input;
	new_input = ft_str_replace(tmp, " <  < ", " << ");
	free(tmp);
	// tmp = new_input;
	// new_input = ft_str_replace(tmp, "\"", " \" ");
	// free(tmp);
	// tmp = new_input;
	// new_input = ft_str_replace(tmp, "\'", " \' ");
	// free(tmp);
	return (new_input);
}

/* --- Process one iteration of the shell loop --- */

// static int	process_shell_iteration(void)
// {
// 	t_shell	*shell;
// 	char	**commands;
// 	int		num_commands;
// 	char	*input;

// 	input = readline("\033[1;32mminishell>\033[0m ");
// 	if (!input)
// 	{
// 		printf("\033[1;31mexit\033[0m\n");
// 		return (0);
// 	}
// 	shell = malloc(sizeof(t_shell));
// 	if (*input)
// 		add_history(input);
// 	input = preprocess_input(input);
// 	commands = ft_split(input, '|');
// 	num_commands = count_words(input, '|');
// 	shell->parser = allocate_shell_commands(num_commands, commands);
// 	print_shell(shell->parser);
// 	free_shell(shell->parser);
// 	free_str_array(commands);
// 	free(input);
// 	return (1);
// }

// /* --- Loop helper --- */

// static void	run_shell_loop(void)
// {
// 	while (process_shell_iteration())
// 		;
// 	clear_history();
// }

// int	main(void)
// {
// 	run_shell_loop();
// 	return (0);
// }
