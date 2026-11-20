/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 15:58:52 by jose-cad          #+#    #+#             */
/*   Updated: 2025/12/01 15:58:53 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_infile(char *filename);
static int	apply_heredoc(t_ast *node);
static int	apply_node_redirection(t_ast *node, t_shell *shell);

int	apply_redirections(t_ast *node, t_shell *shell)
{
	int	status;

	if (!node || node->type == NODE_SUBSHELL)
		return (0);
	if (node->left)
	{
		if (apply_redirections(node->left, shell) != 0)
			return (1);
	}
	status = apply_node_redirection(node, shell);
	if (node->right && status == 0)
		status = apply_redirections(node->right, shell);
	return (status);
}

static int	apply_node_redirection(t_ast *node, t_shell *shell)
{
	char	*file;
	int		status;

	if (node->type == NODE_HEREDOC)
		return (apply_heredoc(node));
	if (node->type != NODE_REDIR_IN && node->type != NODE_REDIR_OUT
		&& node->type != NODE_REDIR_APPEND && node->type != NODE_REDIR_ERR)
		return (0);
	file = resolve_redir_file(node, shell);
	if (!file)
		return (-1);
	if (node->type == NODE_REDIR_IN)
		status = open_infile(file);
	else if (node->type == NODE_REDIR_OUT)
		status = open_outfile_trunc(file);
	else if (node->type == NODE_REDIR_APPEND)
		status = open_outfile_append(file);
	else
		status = open_errfile_trunc(file);
	free(file);
	return (status);
}

static int	apply_heredoc(t_ast *node)
{
	if (node->heredoc_fd < 0)
	{
		ft_dprintf(2, "minishell: internal heredoc error (invalid fd)\n");
		return (-1);
	}
	if (dup2(node->heredoc_fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(node->heredoc_fd);
		node->heredoc_fd = -1;
		return (-1);
	}
	close(node->heredoc_fd);
	node->heredoc_fd = -1;
	return (0);
}

static int	open_infile(char *filename)
{
	int		fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(filename);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}
