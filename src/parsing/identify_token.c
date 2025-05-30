/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identify_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 07:16:41 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/15 18:08:11 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_token_type	identify_redirection_tokens(char *token,
		t_next_token *decide)
{
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
	if (ft_strcmp(token, "<") == 0)
	{
		decide->input_file = 1;
		return (REDIRECT_IN);
	}
	return (ARGUMENT);
}

/* --- Helper to identify special tokens --- */
static t_token_type	identify_special_tokens(char *token)
{
	size_t	len;

	len = ft_strlen(token);
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
	return (ARGUMENT);
}

/* --- Helper to identify sequential tokens (like input/output files,
	commands) --- */
static t_token_type	identify_sequential_tokens(t_next_token *decide)
{
	if (decide->here_doc == 1)
	{
		decide->here_doc = 0;
		return (HEREDOC_DELI);
	}
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

/* --- Main function to identify token type --- */
t_token_type	identify_token_type(char *token, t_next_token *decide)
{
	t_token_type	type;

	if (ft_strcmp(token, "|") == 0)
	{
		decide->command = 1;
		return (PIPE);
	}
	type = identify_redirection_tokens(token, decide);
	if (type != ARGUMENT)
		return (type);
	type = identify_sequential_tokens(decide);
	if (type != ARGUMENT)
		return (type);
	return (identify_special_tokens(token));
}

const char	*get_token_type_name(t_token_type type)
{
	const char	*names[14];

	names[0] = "COMMAND";
	names[1] = "ARGUMENT";
	names[2] = "PIPE";
	names[3] = "REDIRECT_IN";
	names[4] = "REDIRECT_OUT";
	names[5] = "REDIRECT_APPEND";
	names[6] = "HEREDOC";
	names[7] = "HEREDOC_DELI";
	names[8] = "ENV_VAR";
	names[9] = "DOLLAR_SIGN";
	names[10] = "SINGLE_QUOTE";
	names[11] = "DOUBLE_QUOTE";
	names[12] = "INPUT_FILE";
	names[13] = "OUTPUT_FILE";
	return (names[type]);
}
