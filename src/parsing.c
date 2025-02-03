/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@std.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 01:23:07 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/03 16:31:25 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <errno.h>
# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <bits/sigaction.h>


typedef enum e_token_type
{
	COMMAND,
	ARGUMENT,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	REDIRECT_APPEND,
	HEREDOC,
	ENV_VAR,
	DOLLAR_SIGN,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
	INPUT_FILE,
	OUTPUT_FILE,
	LOGICAL_AND,
	LOGICAL_OR,
	ESCAPED_CHAR
}	t_token_type;

typedef struct s_next_token
{
	int	command;
	int	argument;
	int	env_var;
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
	free(tokens);
	return (count);
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

void	free_shell(t_shell *shell)
{
	int	i;
	int	j;

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
		decide->input_file = 1;
		return (REDIRECT_APPEND);
	}
	if (ft_strcmp(token, ">") == 0)
	{
		decide->output_file = 1;
		return (REDIRECT_OUT);
	}
	if (ft_strcmp(token, "<<") == 0)
	{
		decide->output_file = 1;
		return (HEREDOC);
	}
	if (ft_strcmp(token, "<") == 0)
	{
		decide->input_file = 1;
		return (REDIRECT_IN);
	}
	if (token[0] == '$')
    {
        if(len == 1)
            return DOLLAR_SIGN;
		return (ENV_VAR);
    }
	if (token[0] == '\'')
		return (SINGLE_QUOTE);
	if (token[0] == '"')
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
        decide->command=0;
		return (COMMAND);
	}
	return (ARGUMENT);
}

static int initialize_command_tokens(int cmd_index, t_shell *shell, char **tokens, t_next_token *token_state)
{
	int token_index = 0;
	
	while (tokens[token_index])
	{
		shell->commands[cmd_index]->tokens[token_index] = malloc(sizeof(t_token));
		if (!shell->commands[cmd_index]->tokens[token_index])
			return (0);
		shell->commands[cmd_index]->tokens[token_index]->value = ft_strdup(tokens[token_index]);
		if (!shell->commands[cmd_index]->tokens[token_index]->value)
			return (0);
		shell->commands[cmd_index]->tokens[token_index]->type = identify_token_type(tokens[token_index], token_state);
		token_index++;
	}
	return token_index;
}

static int allocate_command_structure(t_shell *shell, int cmd_index, char **command_strings)
{
	t_next_token token_state = {1, 0, 0, 0, 0};
	int token_count;
	char **tokens;

	shell->commands[cmd_index] = malloc(sizeof(t_command));
	if (!shell->commands[cmd_index])
		return (0);
	shell->commands[cmd_index]->token_count = count_words(command_strings[cmd_index], ' ');
	shell->commands[cmd_index]->tokens = malloc(sizeof(t_token *) * (shell->commands[cmd_index]->token_count + 1));
	if (!shell->commands[cmd_index]->tokens)
		return (0);
	tokens = ft_split(command_strings[cmd_index], ' ');
	if (!tokens)
		return (0);
	token_count = initialize_command_tokens(cmd_index, shell, tokens, &token_state);
	if (token_count == 0)
	{
		free_str_array(tokens);
		return (0);
	}
	shell->commands[cmd_index]->tokens[token_count] = NULL;
	free_str_array(tokens);
	return (1);
}

t_shell *allocate_shell_commands(int command_count, char **command_strings)
{
	t_shell *shell;
	int cmd_index;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->commands = malloc(sizeof(t_command *) * (command_count + 1));
	if (!shell->commands)
	{
		free(shell);
		return (NULL);
	}
	shell->command_count = command_count;
	for (cmd_index = 0; cmd_index < command_count; cmd_index++)
	{
		if (!allocate_command_structure(shell, cmd_index, command_strings))
		{
			free_shell(shell);
			return (NULL);
		}
	}
	shell->commands[command_count] = NULL;
	return shell;
}



const char *get_token_type_name(t_token_type type)
{
	return (char *[]){"COMMAND",
	"ARGUMENT",
	"PIPE",
	"REDIRECT_IN",
	"REDIRECT_OUT",
	"REDIRECT_APPEND",
	"HEREDOC",
	"ENV_VAR",
	"DOLLAR_SIGN",
	"SINGLE_QUOTE",
	"DOUBLE_QUOTE",
	"INPUT_FILE",
	"OUTPUT_FILE",
	"UNKNOWN"}[type];
}

void print_shell(t_shell *shell)
{
    int i;
    int j;

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

	new_input = ft_str_replace(input, ">>", " >> ");
	tmp = new_input;
	new_input = ft_str_replace(tmp, "<<", " << ");
	free(tmp);
	tmp = new_input;
	new_input = ft_str_replace(tmp, ">", " > ");
	free(tmp);
	tmp = new_input;
	new_input = ft_str_replace(tmp, "<", " < ");
	free(tmp);
	return (new_input);
}


// int	main(void)
// {
// 	t_shell	*shell;
// 	char	**commands;
// 	int		num_commands;
// 	char	*input;

// 	while (1)
// 	{
// 		input = readline("\033[1;32mminishell>\033[0m ");
// 		if (!input)
// 		{
// 			printf("\033[1;31mexit\033[0m\n");
// 			break ;
// 		}
// 		if (*input)
// 			add_history(input);
// 		input = preprocess_input(input);
// 		commands = ft_split(input, '|');
// 		num_commands = count_words(input, '|');
// 		shell = allocate_shell_commands(num_commands, commands);
//         print_shell(shell);
// 		free_shell(shell);
// 		free(commands);
// 		free(input);
// 	}
// 	clear_history();
// 	return (0);
// }
