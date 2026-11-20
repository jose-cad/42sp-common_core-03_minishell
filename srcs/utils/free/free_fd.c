/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 16:00:19 by jose-cad          #+#    #+#             */
/*   Updated: 2025/12/01 16:00:21 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_heredoc_fds(t_ast *node)
{
	if (!node)
		return ;
	if (node->type == NODE_HEREDOC && node->heredoc_fd >= 0)
	{
		close(node->heredoc_fd);
		node->heredoc_fd = -1;
	}
	close_heredoc_fds(node->left);
	close_heredoc_fds(node->right);
}

void	close_saved_fds(t_shell *shell)
{
	int	i;

	i = 0;
	while (i < 3)
	{
		safe_close(shell->saved_fds[i]);
		shell->saved_fds[i] = -1;
		i++;
	}
}

void	close_std_fds(void)
{
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}
