/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ahramada.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahramada <ahramada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 20:30:00 by ahramada          #+#    #+#             */
/*   Updated: 2025/02/01 19:59:48 by ahramada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <errno.h>
# include <string.h>
# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>

/* Enum for Token Types */
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

/* Struct for Token Decision Making */
typedef struct s_next_token
{
	int	command;
	int	argument;
	int	env_var;
	int	input_file;
	int	output_file;
}	t_next_token;

/* Struct for Token Representation */
typedef struct s_token
{
	char			*value;
	t_token_type	type;
}	t_token;

/* Struct for Command Representation */
typedef struct s_command
{
	t_token	**tokens;
	int		token_count;
}	t_command;

/* Struct for Shell Representation */
typedef struct s_shell
{
	t_command	**commands;
	int			command_count;
}	t_shell;

/* Function Prototypes */
int				count_words(char *str, char sep);
t_token_type	identify_token_type(char *token, t_next_token *decide);
t_shell			*allocate_shell_commands(int num_commands, char **shell_command);
void			free_shell(t_shell *shell);
const char		*get_token_type_name(t_token_type type);
void			print_shell(t_shell *shell);
char			*ft_str_replace(const char *str, const char *old, const char *new);
char			*preprocess_input(char *input);

#endif /* PARSING_H */
