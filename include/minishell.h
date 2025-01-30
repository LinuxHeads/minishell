/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 19:24:00 by abdsalah          #+#    #+#             */
/*   Updated: 2025/01/30 02:55:01 by abdsalah         ###   ########.fr       */
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
    int     exit_status;    // Exit status of the last command executed (stores $? value)
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
void        ft_exit(void);

/* 
** ft_echo: Built-in echo command to print arguments to stdout.
*/
void        ft_echo(void);

/* 
** ft_env: Built-in env command to print environment variables to stdout.
*/
void        ft_env(void);

/* 
** ft_cd: Built-in cd command to change the current directory.
*/
void        ft_cd(char *args, char **envp);

/* 
** ft_export: Built-in export command to set an environment variable.
*/
void        ft_export(void);

/* 
** ft_unset: Built-in unset command to remove an environment variable.
*/
void        ft_unset(void);

/* 
** ft_setenv: Function to manually set an environment variable (can be used 
** to implement `setenv` if needed).
*/
void        ft_setenv(const char *name, const char *value, t_env **env_list);

/* 
** ft_pwd: Built-in pwd command to print the current directory path.
*/
void        ft_pwd(void);

/* ************************************************************************** */
/*                            MINISHELL INITIALIZATION                         */
/* ************************************************************************** */

/* 
** init_minishell: Initializes the shell environment by setting up the environment
** variables and initializing other necessary components.
*/
void        init_minishell(t_shell *shell, char **envp);

/* ************************************************************************** */

#endif
