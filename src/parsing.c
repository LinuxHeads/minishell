/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 01:23:07 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/01 18:51:08 by abdsalah         ###   ########.fr       */
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

t_shell	*allocate_shell_commands(int num_commands, char **shell_command)
{
	t_shell			*shell;
	t_next_token	decide;
	int				i;
	int				j;
	char			**token_str;

	i = 0;
	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->commands = malloc(sizeof(t_command *) * (num_commands + 1));
	if (!shell->commands)
		return (NULL);
	shell->command_count = num_commands;
	while (i < num_commands)
	{
        decide = (t_next_token){1, 0, 0, 0, 0};
		shell->commands[i] = malloc(sizeof(t_command));
		if (!shell->commands[i])
			return (NULL);
		shell->commands[i]->token_count = count_words(shell_command[i], ' ');
		shell->commands[i]->tokens = malloc(sizeof(t_token *)
				* (shell->commands[i]->token_count + 1));
		token_str = ft_split(shell_command[i], ' ');
		j = 0;
		while (token_str[j])
		{
			shell->commands[i]->tokens[j] = malloc(sizeof(t_token));
			shell->commands[i]->tokens[j]->value = ft_strdup(token_str[j]);
			shell->commands[i]->tokens[j]->type = identify_token_type(token_str[j], &decide);
			j++;
		}
		shell->commands[i]->tokens[j] = NULL;
		free(token_str);
		i++;
	}
	return (shell);
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
const char *get_token_type_name(t_token_type type)
{
    // switch (type)
    // {
    //     case COMMAND: return "COMMAND";
    //     case ARGUMENT: return "ARGUMENT";
    //     case PIPE: return "PIPE";
    //     case REDIRECT_IN: return "REDIRECT_IN";
    //     case REDIRECT_OUT: return "REDIRECT_OUT";
    //     case REDIRECT_APPEND: return "REDIRECT_APPEND";
    //     case HEREDOC: return "HEREDOC";
    //     case ENV_VAR: return "ENV_VAR";
    //     case DOLLAR_SIGN: return "DOLLAR_SIGN";
    //     case SINGLE_QUOTE: return "SINGLE_QUOTE";
    //     case DOUBLE_QUOTE: return "DOUBLE_QUOTE";
    //     case INPUT_FILE: return "INPUT_FILE";
    //     case OUTPUT_FILE: return "OUTPUT_FILE";
    //     default: return "UNKNOWN";
    // }
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

char	*ft_str_replace(const char *str, const char *old, const char *new)
{
	char	*result;
	int		count;
	int		new_len;
	int		old_len;
	int		i;
	int		j;

	if (!str || !old || !new)
		return (NULL);
	count = 0;
	old_len = ft_strlen(old);
	new_len = ft_strlen(new);
	i = 0;
	while (str[i])
	{
		if (ft_strncmp(&str[i], old, old_len) == 0)
		{
			count++;
			i += old_len;
		}
		else
			i++;
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
			ft_memcpy(&result[j], new, new_len);
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
		free(commands);
		free(input);
	}
	clear_history();
	return (0);
}
