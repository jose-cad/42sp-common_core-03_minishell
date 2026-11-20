/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 16:03:19 by jose-cad          #+#    #+#             */
/*   Updated: 2025/12/01 16:03:21 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*get_command_node(t_ast *node)
{
	while (node && node->type != NODE_COMMAND)
		node = node->left;
	return (node);
}

t_ast	*skip_redirs(t_ast *node)
{
	while (node->left && (node->type == NODE_REDIR_IN
			|| node->type == NODE_REDIR_OUT
			|| node->type == NODE_REDIR_APPEND
			|| node->type == NODE_HEREDOC
			|| node->type == NODE_REDIR_ERR))
		node = node->left;
	return (node);
}

int	save_std_fds(int *sv_in, int *sv_out, int *sv_err)
{
	*sv_in = dup(STDIN_FILENO);
	*sv_out = dup(STDOUT_FILENO);
	*sv_err = dup(STDERR_FILENO);
	if (*sv_in == -1 || *sv_out == -1 || *sv_err == -1)
	{
		perror("dup");
		safe_close(*sv_in);
		safe_close(*sv_out);
		safe_close(*sv_err);
		*sv_in = -1;
		*sv_out = -1;
		*sv_err = -1;
		return (-1);
	}
	return (0);
}

void	restore_std_fds(int sv_in, int sv_out, int sv_err)
{
	if (sv_in >= 0)
	{
		dup2(sv_in, STDIN_FILENO);
		close(sv_in);
	}
	if (sv_out >= 0)
	{
		dup2(sv_out, STDOUT_FILENO);
		close(sv_out);
	}
	if (sv_err >= 0)
	{
		dup2(sv_err, STDERR_FILENO);
		close(sv_err);
	}
}
