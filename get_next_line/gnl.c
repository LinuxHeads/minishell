/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 08:53:07 by abdsalah          #+#    #+#             */
/*   Updated: 2025/01/26 20:06:29 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/get_next_line.h"

static void	*ft_free_buffers(char **global_buffer, char *read_buffer)
{
	if (global_buffer && *global_buffer)
	{
		free(*global_buffer);
		*global_buffer = NULL;
	}
	if (read_buffer)
		free(read_buffer);
	return (NULL);
}

static char	*ft_read_into_buffer(char **global_buffer, int fd)
{
	int		read_value;
	char	*read_buffer;
	char	*temp_buffer;

	read_buffer = malloc(BUFFER_SIZE + 1);
	if (!read_buffer)
		return (ft_free_buffers(global_buffer, NULL));
	while (!ft_strchr(*global_buffer, '\n'))
	{
		read_value = read(fd, read_buffer, BUFFER_SIZE);
		if (read_value == -1)
			return (ft_free_buffers(global_buffer, read_buffer));
		if (**global_buffer == '\0' && read_value == 0)
			return (ft_free_buffers(NULL, read_buffer));
		if (read_value == 0)
			break ;
		read_buffer[read_value] = '\0';
		temp_buffer = ft_strjoin(*global_buffer, read_buffer);
		if (!temp_buffer)
			return (ft_free_buffers(global_buffer, read_buffer));
		free(*global_buffer);
		*global_buffer = temp_buffer;
	}
	free(read_buffer);
	return (*global_buffer);
}

static char	*ft_extract_line(char **global_buffer)
{
	char	*pos;
	char	*str;
	int		offset;
	char	*temp_global_buffer;

	pos = ft_strchr(*global_buffer, '\n');
	if (pos)
		offset = pos - *global_buffer + 1;
	else
		offset = ft_strlen(*global_buffer);
	str = malloc(offset + 1);
	if (!str)
		return (ft_free_buffers(global_buffer, NULL));
	ft_strncpy(str, *global_buffer, offset);
	str[offset] = '\0';
	temp_global_buffer = ft_strdup(*global_buffer + offset);
	if (!temp_global_buffer)
	{
		free(str);
		return (ft_free_buffers(global_buffer, NULL));
	}
	free(*global_buffer);
	*global_buffer = temp_global_buffer;
	return (str);
}

char	*get_next_line(int fd)
{
	static char	*global_buffer = NULL;
	char		*str;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!global_buffer)
	{
		global_buffer = ft_strdup("");
		if (!global_buffer)
			return (NULL);
	}
	if (!ft_read_into_buffer(&global_buffer, fd))
		return (ft_free_buffers(&global_buffer, NULL));
	str = ft_extract_line(&global_buffer);
	return (str);
}
