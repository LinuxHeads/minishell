/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@std.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 01:23:07 by abdsalah          #+#    #+#             */
/*   Updated: 2025/01/31 16:51:21 by abdsalah         ###   ########.fr       */
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
    REDIRECT_IN,        // <
    REDIRECT_OUT,       // >
    REDIRECT_APPEND,    // >>
    HEREDOC,            // <<
    ENV_VAR,            // $VAR
    DOLLAR_SIGN,      	// $
    SINGLE_QUOTE_START, // '
    SINGLE_QUOTE_END,   // '
    DOUBLE_QUOTE_START, // "
    DOUBLE_QUOTE_END,    // "
	INPUT_FILE,
	OUTPUT_FILE
}   t_token_type;



typedef struct s_token
{
    char *value;
    t_token_type type;
}   t_token;

typedef struct s_command
{
    t_token **tokens;  
    int token_count;   
}   t_command;

typedef struct s_shell
{
    t_command **commands;  
    int command_count;     
}   t_shell;

t_token_type identify_token_type(char *token)
{
    size_t len = ft_strlen(token);

    if (ft_strcmp(token, "|") == 0)
        return PIPE;
    if (ft_strcmp(token, ">") == 0)
        return REDIRECT_OUT;
    if (ft_strcmp(token, ">>") == 0)
        return REDIRECT_APPEND;
    if (ft_strcmp(token, "<") == 0)
        return REDIRECT_IN;
    if (ft_strcmp(token, "<<") == 0)
        return HEREDOC;


    if (token[0] == '$')
    {
        if (len == 1) 
            return DOLLAR_SIGN;
        return ENV_VAR;
    }


    if (token[0] == '\'')
    {
        if (len == 1) 
            return SINGLE_QUOTE_START;
        if (token[len - 1] == '\'')
            return SINGLE_QUOTE_END;
        return SINGLE_QUOTE_START;
    }


    if (token[0] == '"')
    {
        if (len == 1)
            return DOUBLE_QUOTE_START;
        if (token[len - 1] == '"')
            return DOUBLE_QUOTE_END;
        return DOUBLE_QUOTE_START;
    }

    return COMMAND;
}


int count_words(char *str, char sep)
{
    int count;
    char **tokens;

    count = 0;
    tokens = ft_split(str, sep);
    while (tokens[count])
        count++;
    free(tokens);
    return (count);
}

t_shell *allocate_shell_commands(int num_commands, char **shell_command)
{
    t_shell *shell;
    int i;
    int j;
    char **token_str;

    shell = malloc(sizeof(t_shell));
    if (!shell)
        return (NULL);
    shell->commands = malloc(sizeof(t_command *) * (num_commands + 1));
    if (!shell->commands)
        return (NULL);
    shell->command_count = num_commands;
    i = 0;
    while (i < num_commands)
    {
        shell->commands[i] = malloc(sizeof(t_command));
        shell->commands[i]->token_count = count_words(shell_command[i], ' ');
        shell->commands[i]->tokens = malloc(sizeof(t_token *) * (shell->commands[i]->token_count + 1));
        token_str = ft_split(shell_command[i], ' ');
        j = 0;
	while (token_str[j])
	{
    	shell->commands[i]->tokens[j] = malloc(sizeof(t_token));
    	shell->commands[i]->tokens[j]->value = ft_strdup(token_str[j]);
    	shell->commands[i]->tokens[j]->type = identify_token_type(token_str[j]);
    	if (j > 0)
    	{
        	if (shell->commands[i]->tokens[j - 1]->type == REDIRECT_IN)
            	shell->commands[i]->tokens[j]->type = INPUT_FILE;
        	else if (shell->commands[i]->tokens[j - 1]->type == REDIRECT_OUT ||
            	shell->commands[i]->tokens[j - 1]->type == REDIRECT_APPEND)
            	shell->commands[i]->tokens[j]->type = OUTPUT_FILE;
    	}
    	j++;
	}

        shell->commands[i]->tokens[j] = NULL;
        free(token_str);
        i++;
    }
    return shell;
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
            printf("  Token %d: %s (Type: %d)\n", j + 1,
                   shell->commands[i]->tokens[j]->value,
                   shell->commands[i]->tokens[j]->type);
            j++;
        }
        i++;
    }
}

void free_shell(t_shell *shell)
{
    int i;
    int j;

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


// int main(void)
// {
//     t_shell *shell;
//     char **commands;
//     int num_commands;
//     char *input;

//     while (1)
//     {
//         /* \033[1;32m sets bold green color, \033[0m resets all attributes */
//         input = readline("\033[1;32mminishell>\033[0m ");
        
//         if (!input)
//         {
//             /* \033[1;31m sets bold red color, \033[0m resets all attributes */
//             printf("\033[1;31mexit\033[0m\n");
//             break;
//         }
        
//         if (*input)
//             add_history(input);
        
//         commands = ft_split(input, '|');
//         num_commands = count_words(input, '|');

//         shell = allocate_shell_commands(num_commands, commands);
//         print_shell(shell);

//         free_shell(shell);
//         free(commands);
//         free(input);
//     }

//     clear_history();
//     return 0;
// }
