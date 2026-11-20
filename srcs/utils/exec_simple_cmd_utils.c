/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple_cmd_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 16:03:14 by jose-cad          #+#    #+#             */
/*   Updated: 2025/12/01 16:03:16 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_empty_after_expand(t_ast *node, t_shell *shell,
				t_ast *cmd_node)
{
	(void)cmd_node;
	if (has_redirections(node))
		return (handle_redirect_only(node, shell));
	shell->exit_status = 0;
	return (0);
}

int	process_command(t_ast *node, t_shell *shell, t_ast *cmd_node)
{
	if (handle_assignment_only(cmd_node, shell))
		return (shell->exit_status);
	if (is_builtin(cmd_node))
	{
		if (!has_redirections(node))
			shell->exit_status = exec_builtin(cmd_node, shell);
		else
			shell->exit_status = exec_builtin_parent(node, shell);
		return (shell->exit_status);
	}
	return (execute_forked_cmd(node, shell));
}

int	handle_redirect_only(t_ast *node, t_shell *shell)
{
	int	sv_in;
	int	sv_out;
	int	sv_err;
	int	status;

	if (save_std_fds(&sv_in, &sv_out, &sv_err) == -1)
		return (1);
	status = apply_redirections(node, shell);
	restore_std_fds(sv_in, sv_out, sv_err);
	if (status != 0)
	{
		shell->exit_status = 1;
		return (1);
	}
	shell->exit_status = 0;
	return (0);
}

int	is_empty_command(char **argv)
{
	return (!argv || !argv[0]);
}

int	exec_builtin_parent(t_ast *node, t_shell *shell)
{
	int		*sv;
	int		status;
	t_ast	*cmd_node;

	sv = shell->saved_fds;
	if (save_std_fds(&sv[0], &sv[1], &sv[2]) == -1)
		return (1);
	status = 1;
	cmd_node = get_command_node(node);
	if (apply_redirections(node, shell) == 0 && cmd_node)
		status = exec_builtin(cmd_node, shell);
	restore_std_fds(sv[0], sv[1], sv[2]);
	sv[0] = -1;
	sv[1] = -1;
	sv[2] = -1;
	return (status);
}
