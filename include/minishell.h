/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 19:24:00 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/07 18:45:56 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* Standard library includes */
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <errno.h>

/* Libft and Readline library includes */
# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <bits/sigaction.h>

/* ************************************************************************** */
/*                            STRUCTS                                          */
/* ************************************************************************** */

/* 
** s_env: represents a single environment variable in the shell, with a name
** and value. The linked list structure allows dynamic management of environment
** variables.
*/
extern volatile int g_signal_flag;

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


typedef struct t_exec
{
	t_command	**commands;
	int			command_count;
}	t_exec;

typedef struct s_env
{
    char	*name;         // Name of the environment variable
    char	*value;        // Value of the environment variable
    struct s_env	*next;   // Pointer to the next environment variable (linked list)
} t_env;

/* 
** s_shell: contains shell-wide state information. Includes a linked list of
** environment variables and an array representation of the environment, as well
** as the exit status of the last executed command.
*/
typedef struct s_shell
{
    t_env   *env_list;      // Linked list of environment variables
    char    **envp;         // Array representation for execve
    int     exit_status;
	t_exec	*parser;   // Exit status of the last command executed (stores $? value)
} t_shell;



/* ************************************************************************** */
/*                            SIGNAL HANDLING                                   */
/* ************************************************************************** */

/* 
** signals_t3res: Sets up signal handling for interactive shell. Handles SIGINT 
** (Ctrl+C) by calling the function `handle_sigint` and ignoring SIGQUIT (Ctrl+\).
*/
void        signals_t3res(void);

/* ************************************************************************** */
/*                            ENVIRONMENT VARIABLES                           */
/* ************************************************************************** */

/* 
** envp_to_str: Converts the environment linked list to an array of strings, 
** each string representing an environment variable in "name=value" format.
*/
char        **envp_to_str(t_env *env);

/* 
** print_envp: Prints the environment variables in the linked list format.
*/
void        print_envp(t_env *env);

/* 
** printstr_envp: Prints the environment variables in the array format (each 
** string printed on a new line).
*/
void        printstr_envp(char **envp);

/* 
** init_envp: Initializes the environment linked list from the passed environment 
** variables (envp) array.
*/
t_env       *init_envp(char **envp);

/* 
** free_envp_list: Frees the memory allocated for the environment linked list.
*/
void        free_envp_list(t_env *env);

/* 
** free_envp_array: Frees the memory allocated for the environment array (envp).
*/
void        free_envp_array(char **envp);

/* 
** env_length: Returns the number of environment variables in the linked list.
*/
int         env_length(t_env *env);

/* ************************************************************************** */
/*                            BUILTIN COMMANDS                                 */
/* ************************************************************************** */

/* 
** ft_exit: Built-in exit command to terminate the shell.
*/
int ft_exit(char **args, t_shell *shell);

/* 
** ft_echo: Built-in echo command to print arguments to stdout.
*/
int ft_echo(char **args);

/* 
** ft_env: Built-in env command to print environment variables to stdout.
*/
int    ft_env(char **args, t_shell *shell);

/* 
** ft_cd: Built-in cd command to change the current directory.
*/
int        ft_cd(char **args, t_env **envp);

/* 
** ft_export: Built-in export command to set an environment variable.
*/
int        ft_export(char **args, t_env **env);

/* 
** ft_unset: Built-in unset command to remove an environment variable.
*/
int    ft_unset(char **arg, t_env **envp);

/* 
** ft_setenv: Function to manually set an environment variable (can be used 
** to implement `setenv` if needed).
*/
int		ft_setenv(const char *name, const char *value, t_env **env_list);

int	expander(char ***argv_ptr, t_shell *shell);
void	expander_test(char **argv, t_shell *shell);

/* 
** ft_pwd: Built-in pwd command to print the current directory path.
*/
int        ft_pwd(void);

/* ************************************************************************** */
/*                            MINISHELL INITIALIZATION                         */
/* ************************************************************************** */

/* 
** init_minishell: Initializes the shell environment by setting up the environment
** variables and initializing other necessary components.
*/
void        init_minishell(t_shell *shell, char **envp);

/* ************************************************************************** */
char		*ft_getenv(const char *name, t_env *env);


/* Function Prototypes */
int				count_words(char *str, char sep);
t_token_type	identify_token_type(char *token, t_next_token *decide);
t_exec			*allocate_shell_commands(int num_commands, char **shell_command);
void			free_shell(t_exec *shell);
const char		*get_token_type_name(t_token_type type);
void			print_shell(t_exec *shell);
char			*ft_str_replace(const char *str, const char *old, const char *);
char			*preprocess_input(char *input);
void			free_str_array(char **arr);
void			execute_pipeline(t_shell **shell);
void			signals_t3res(void);
void			free_envp_node(t_env *env);
char			**build_command_argv(t_command *cmd);
char			*find_command_path(char *cmd, char **envp);
int				is_builtin(char **arg);
int				exec_builtins(char **args, t_shell *shell);
char			**ft_splitter(const char *s, char c);
int				ft_setup_shlvl(t_env **envp);
void			free_shell(t_exec *shell);
#endif
