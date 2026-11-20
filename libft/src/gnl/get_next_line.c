/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@student.42sp.org.br>     +#+  +:+       +#+       */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 21:45:00 by jose-cad          #+#    #+#             */
/*   Updated: 2025/07/27 22:00:00 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	find_line_end(t_gnl *s, int fd);

char	*get_next_line(int fd)
{
	static t_gnl	s[GNL_MAX_FD];
	char			*line;
	size_t			end;

	if (fd < 0 || fd >= GNL_MAX_FD || GNL_SIZE <= 0)
		return (NULL);
	end = find_line_end(&s[fd], fd);
	if (end == s[fd].start)
		return (gnl_reset(&s[fd]));
	line = malloc(end - s[fd].start + 1);
	if (!line)
		return (gnl_reset(&s[fd]));
	gnl_copy(line, s[fd].buf + s[fd].start, end - s[fd].start);
	line[end - s[fd].start] = '\0';
	s[fd].start = end;
	if (s[fd].start == s[fd].len)
		gnl_reset(&s[fd]);
	return (line);
}

static size_t	find_line_end(t_gnl *s, int fd)
{
	size_t	i;
	ssize_t	bytes_read;

	i = s->start;
	while (1)
	{
		while (i < s->len)
		{
			if (s->buf[i++] == '\n')
				return (i);
		}
		if (!gnl_grow(s))
			break ;
		i = s->len;
		bytes_read = read(fd, s->buf + s->len, GNL_SIZE);
		if (bytes_read < 0)
			break ;
		if (bytes_read == 0)
			return (s->len);
		s->len += bytes_read;
	}
	gnl_reset(s);
	return (0);
}
