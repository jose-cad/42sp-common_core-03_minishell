/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:00:13 by jose-cad          #+#    #+#             */
/*   Updated: 2025/12/06 00:39:15 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*heredoc_path(int n)
{
	char	*num;
	char	*path;

	num = ft_itoa(n);
	if (!num)
		return (NULL);
	path = ft_strjoin("/tmp/.minishell_heredoc_", num);
	free(num);
	return (path);
}

int	open_heredoc_file(int *fds)
{
	int		n;
	char	*path;

	n = 0;
	fds[1] = -1;
	while (fds[1] < 0)
	{
		path = heredoc_path(n++);
		if (!path)
			return (-1);
		if (access(path, F_OK) == 0)
		{
			free(path);
			continue ;
		}
		fds[1] = open(path, O_WRONLY | O_CREAT | O_EXCL, 0600);
		if (fds[1] < 0)
			return (free(path), -1);
	}
	fds[0] = open(path, O_RDONLY);
	unlink(path);
	free(path);
	if (fds[0] < 0)
		return (close(fds[1]), -1);
	return (0);
}

int	handle_heredoc_interrupt(int *pipe_fds)
{
	close(pipe_fds[0]);
	close(pipe_fds[1]);
	setup_signals();
	return (130);
}

int	process_heredoc_line(char *line, t_ast *node, int write_fd, t_shell *shell)
{
	size_t	len_line;
	size_t	len_delim;
	char	*expanded_line;

	len_line = ft_strlen(line);
	len_delim = ft_strlen(node->filename);
	if (ft_strncmp(line, node->filename, len_delim + 1) == 0
		&& len_line == len_delim)
	{
		free(line);
		return (1);
	}
	if (node->heredoc_expand)
		expanded_line = expand_heredoc_line(line, shell);
	else
		expanded_line = line;
	ft_putendl_fd(expanded_line, write_fd);
	if (node->heredoc_expand)
		free(expanded_line);
	free(line);
	return (0);
}

char	*read_heredoc_line(void)
{
	t_buf	buf;
	char	ch;
	int		bytes_read;

	buf.len = 0;
	buf.cap = 128;
	buf.str = malloc(buf.cap);
	if (!buf.str)
		return (NULL);
	buf.str[0] = '\0';
	while (buf.str)
	{
		bytes_read = read(STDIN_FILENO, &ch, 1);
		if (bytes_read <= 0 && buf.len == 0)
			return (free(buf.str), NULL);
		if (bytes_read <= 0 || ch == '\n')
			break ;
		buf_append(&buf, &ch, 1);
	}
	return (buf.str);
}
