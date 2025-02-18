/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 19:24:00 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/18 05:01:14 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*
** ************************************************************************** **
**                           Standard Library Includes                        **
** ************************************************************************** **
*/

# include <asm-generic/signal-defs.h>
# include <errno.h>
# include <fcntl.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

/*
** ************************************************************************** **
**                         Libft and Readline Includes                        **
** ************************************************************************** **
*/

# include "../libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>

/*
** ************************************************************************** **
**                                Structures                                  **
** ************************************************************************** **
*/

/*
** Global signal flag.
*/
extern int			g_signal_flag;

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
}					t_token_type;

typedef struct s_next_token
{
	int				command;
	int				argument;
	int				env_var;
	int				here_doc;
	int				input_file;
	int				output_file;
}					t_next_token;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
}					t_token;

typedef struct s_command
{
	t_token			**tokens;
	int				token_count;
}					t_command;

typedef struct s_exec
{
	t_command		**commands;
	int				command_count;
}					t_exec;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_shell
{
	t_env			*env_list;
	char			**envp;
	int				exit_status;
	t_exec			*parser;
	char			*input;
	char			**argv;
	int				in_fd;
	int				out_fd;
	int				prev_fd;
	int				pipe_fd[2];
	int				pipe_created;
}					t_shell;

typedef struct s_exp
{
	const char		*str;
	int				i;
	char			**result;
	int				in_sq;
	int				in_dq;
}					t_exp;

typedef struct s_expander
{
	const char		*str;
	int				i;
	int				in_sq;
	int				in_dq;
	t_shell			*shell;
	char			*result;
}					t_expander;

/*
** ************************************************************************** **
**                         Signal Handling Functions                          **
** ************************************************************************** **
*/

void				signals_setup(int mode);
void				reset_signals(void);
void				reset_signals_heredoc(void);
void				close_heredoc(int signum);

/*
** ************************************************************************** **
**                     Environment Variable Functions                         **
** ************************************************************************** **
*/

char				**envp_to_str(t_env *env);
void				print_envp(t_env *env);
void				printstr_envp(char **envp);
t_env				*init_envp(char **envp);
char				*free_envp_list(t_env *env);
void				free_envp_array(char **envp);
int					env_length(t_env *env);
char				*ft_getenv(const char *name, t_env *env);
int					ft_setenv(const char *name, const char *value,
						t_env **env_list);
int					ft_setup_shlvl(t_env **envp);
t_env				*ft_copy_env(t_env *env);
void				ft_sort_env(t_env **env);
int					ft_lstsize_env(t_env *env_list);
void				free_envp_node(t_env *env);

/*
** ************************************************************************** **
**                         Builtin Command Functions                          **
** ************************************************************************** **
*/

int					ft_exit(char **args, t_shell *shell);
int					ft_echo(char **args);
int					ft_env(char **args, t_shell *shell);
int					ft_cd(char **args, t_env **envp);
int					ft_export(char **args, t_env **env);
int					ft_unset(char **arg, t_env **envp);
int					ft_pwd(void);
int					is_builtin_command(char **arg);
int					execute_builtin_command(char **args, t_shell *shell);

/*
** ************************************************************************** **
**                         Builtin helper					                    **
** ************************************************************************** **
*/

int					export_syntax_error(const char *arg);
int					check_initial_syntax(const char *arg);
int					split_var(const char *arg, char **key, char **value);
int					split_without_equal(const char *arg, char **key,
						char **value);
int					split_without_equal(const char *arg, char **key,
						char **value);
int					appeand_mode(char *key, char *value, t_env **env_list);
int					set_or_append_env(char *key, char *value, t_env **env_list);

/*
** ************************************************************************** **
**                      Minishell Initialization Functions                    **
** ************************************************************************** **
*/

void				init_minishell(t_shell *shell, char **envp);

/*
** ************************************************************************** **
**                        Parsing and Expansion Functions                     **
** ************************************************************************** **
*/

int					count_words(char *str, char sep);
t_token_type		identify_token_type(char *token, t_next_token *decide);
t_exec				*allocate_shell_commands(int num_commands,
						char **shell_command);
void				free_shell(t_exec *shell);
const char			*get_token_type_name(t_token_type type);
void				print_shell(t_exec *shell);
char				*ft_str_replace(const char *str, const char *old,
						const char *new_sub);
char				*preprocess_input(char *input);
void				free_str_array(char **arr);
char				**ft_splitter(const char *s, char c);
int					get_redirections(t_command *cmd, int *in_fd, int *out_fd,
						t_shell *shell);
char				*trim_quotes(char *value);
int					syntax_checker(t_exec *shell);
int					is_double_quote_token(char *s);
int					is_single_quote_token(char *s);
int					count_surrounding_quotes(const char *str);
int					is_whitespace(const char *str);
char				*expand_env_string(const char *str, t_shell *shell);
char				*cleanup_input(char *input);
int					array_length(char **arr);
char				*expand_env_variable(const char *str, int *index,
						t_shell *shell);
void				remove_extra_spaces(char *new_str, const char *str, int *j);
char				*trim_spaces(const char *s);
char				*strip_outers_quotes(const char *s);
char				*compress_whitespace(const char *str);
void				expand_single_argument(char **arg, t_shell *shell);
void				free_tokens(char **arr, int count);
int					check_for_quotes(char *old_arg, char **arg);
int					has_closing(int i, char quote, const char *s, size_t len);
int					check_for_sign(char *old_arg, char **arg);
char				**replace_token_in_array(char **argv, int index,
						char **new_tokens);
int					process_env_token(char ***argv_ptr, t_shell *shell, int i);
int					expander(t_shell **shell);
t_exp				init_exp(const char *str, char **res);
void				toggle_quotes(char c, int *in_sq, int *in_dq);
char				*expand_name(const char *str, int start, int *index,
						t_shell *shell);
int					expand_digit(char **expanded, t_shell *shell, int *index,
						const char *str);
int					normalize_env_value(char **var_value, char *result);
char				*process_var_value_quotes(char *var_value);
char				*process_var_value_spaces(char *var_value, int in_dq,
						char **result);
int					handle_dollar(t_expander *ctx);
int					append_char(t_expander *ctx);
/*
** ************************************************************************** **
**                         Execution Functions                                **
** ************************************************************************** **
*/

void				execute_pipeline(t_shell **shell);
char				**build_command_argv(t_command *cmd);
char				*find_command_path(char *cmd, char **envp);
void				validate_command_path(char *cmd_path, t_shell **shell);
void				fork_check(int pid, t_shell **shell);
void				exec_in_child(int i, t_shell **shell, int *pid,
						int redir_flag);
void				execute_builtin_in_parent(t_shell **shell, int redir_flag);
void				ft_heredoc(int pipe_fds[2], t_command *cmd, int i,
						t_shell *shell);
void				ft_exit_handler(t_shell *shell, void *ptr, char **message,
						int exit_code);
int					contains_command_token(t_command *cmd);
int					setup_command(t_shell **shell, int i, int *status);
int					process_command(t_shell **shell, int i, int *pid);
void				close_fds(t_shell *shell);
int					handle_signal_and_token(t_shell **shell, int i,
						int *redir_flag);
/*
** ************************************************************************** **
**                        Miscellaneous Functions                             **
** ************************************************************************** **
*/

void				remove_argument(char ***argv_ptr, int index);

#endif
