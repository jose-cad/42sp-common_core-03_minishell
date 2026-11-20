/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 16:00:37 by jose-cad          #+#    #+#             */
/*   Updated: 2025/12/01 16:00:38 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	subshell_child(t_ast *node, t_shell *shell)
{
	setup_signals_child();
	shell->is_child = 1;
	if (apply_redirections(node, shell) != 0)
		clean_exit_child(shell, EXIT_FAILURE);
	execute_ast(skip_redirs(node)->left, shell);
	clean_exit_child(shell, shell->exit_status);
}

int	exec_subshell(t_ast *node, t_shell *shell)
{
	pid_t	pid;
	int		status;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid < 0)
	{
		setup_signals();
		return (perror("fork"), 1);
	}
	if (pid == 0)
		subshell_child(node, shell);
	waitpid(pid, &status, 0);
	print_signal_output(status);
	setup_signals();
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
	return (shell->exit_status);
}
