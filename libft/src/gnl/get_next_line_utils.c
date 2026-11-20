/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@student.42sp.org.br>     +#+  +:+       +#+       */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 21:45:00 by jose-cad          #+#    #+#             */
/*   Updated: 2025/07/27 22:00:00 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*gnl_reset(t_gnl *s)
{
	free(s->buf);
	s->buf = NULL;
	s->start = 0;
	s->len = 0;
	s->cap = 0;
	return (NULL);
}

void	gnl_copy(char *dst, const char *src, size_t n)
{
	while (n--)
		*dst++ = *src++;
}

int	gnl_grow(t_gnl *s)
{
	char	*new_buf;
	size_t	rest;

	if (s->cap - s->len >= (size_t)GNL_SIZE)
		return (1);
	rest = s->len - s->start;
	new_buf = s->buf;
	if (s->cap < rest * 2 + (size_t)GNL_SIZE)
		new_buf = malloc(rest * 2 + (size_t)GNL_SIZE);
	if (!new_buf)
		return (0);
	if (rest)
		gnl_copy(new_buf, s->buf + s->start, rest);
	if (new_buf != s->buf)
	{
		free(s->buf);
		s->buf = new_buf;
		s->cap = rest * 2 + (size_t)GNL_SIZE;
	}
	s->start = 0;
	s->len = rest;
	return (1);
}
