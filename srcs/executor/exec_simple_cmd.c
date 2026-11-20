/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 16:00:31 by jose-cad          #+#    #+#             */
/*   Updated: 2025/12/01 16:00:33 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	expansion_failed(t_shell *shell)
{
	shell->exit_status = 1;
	return (1);
}

int	exec_simple_cmd(t_ast *node, t_shell *shell)
{
	t_ast	*cmd_node;

	if (!node)
		return (1);
	cmd_node = get_command_node(node);
	if (has_redirections(node) && (!cmd_node || !cmd_node->argv
			|| !cmd_node->argv[0]))
		return (handle_redirect_only(node, shell));
	if (!cmd_node || !cmd_node->argv || !cmd_node->argv[0])
		return (0);
	if (expand_argv(cmd_node, shell) != 0)
		return (expansion_failed(shell));
	if (is_empty_command(cmd_node->argv))
		return (handle_empty_after_expand(node, shell, cmd_node));
	if (should_expand_wildcards(cmd_node->argv, cmd_node->argv_quoted)
		&& expand_wildcards(&(cmd_node->argv), cmd_node->argv_quoted) != 0)
		return (expansion_failed(shell));
	if (is_empty_command(cmd_node->argv))
		return (handle_empty_after_expand(node, shell, cmd_node));
	return (process_command(node, shell, cmd_node));
}
