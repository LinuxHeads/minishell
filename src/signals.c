/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 01:23:07 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/15 05:49:19 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void handle_sigint(int sig)
{
    g_signal_flag = sig;
    write(STDOUT_FILENO, "\n", 1);
    close (0);
    
}

// Reset signals in child processes
void reset_signals(void)
{
    struct sigaction sa;

    ft_bzero(&sa, sizeof(sa));
    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
}

// Setup signal handling for interactive mode
void signals_setup(int mode)
{
    struct sigaction sa;
    
    ft_bzero(&sa, sizeof(sa));
    if (!mode)
        sa.sa_handler = handle_sigint;
    else
        sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}
