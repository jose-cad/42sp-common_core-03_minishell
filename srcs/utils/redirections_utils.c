/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 16:03:57 by jose-cad          #+#    #+#             */
/*   Updated: 2025/12/01 16:03:59 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	safe_close(int fd)
{
	if (fd >= 0)
		close(fd);
}

int	has_redirections(t_ast *node)
{
	if (!node)
		return (0);
	if (node->type == NODE_REDIR_IN
		|| node->type == NODE_REDIR_OUT
		|| node->type == NODE_REDIR_APPEND
		|| node->type == NODE_HEREDOC
		|| node->type == NODE_REDIR_ERR)
		return (1);
	return (has_redirections(node->left) || has_redirections(node->right));
}
