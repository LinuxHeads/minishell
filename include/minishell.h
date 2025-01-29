/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 19:24:00 by abdsalah          #+#    #+#             */
/*   Updated: 2025/01/29 23:28:26 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include "../libft/libft.h"

typedef struct s_env
{
    char	*name;
    char	*value;
    struct s_env	*next;
}	t_env;

char        **envp_to_str(t_env *env);
void        print_envp(t_env *env);
t_env       *init_envp(char **envp);
void		ft_exit();
void		ft_echo();
void		ft_env();
void		ft_cd(char *args, char **envp);
void		ft_export();
void		ft_unset();
void        ft_setenv(const char *name, const char *value, char **envp);
void        ft_pwd();
void        init_minishell(char **envp);
void        free_envp_array(char **envp);
void        free_envp_list(t_env *env);
void        printstr_envp(char **envp);
int         env_length(t_env *env);

#endif
