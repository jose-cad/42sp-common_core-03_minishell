/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 10:00:19 by jose-cad          #+#    #+#             */
/*   Updated: 2025/12/13 10:05:21 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_pipe_output(int status_right);

int	parent_wait_and_close(pid_t pid_left, pid_t pid_right, int *pipe_fds)
{
	int	status_right;
	int	status_left;
	int	ret;

	close(pipe_fds[0]);
	close(pipe_fds[1]);
	waitpid(pid_right, &status_right, 0);
	waitpid(pid_left, &status_left, 0);
	print_pipe_output(status_right);
	setup_signals();
	if (WIFEXITED(status_right))
		ret = WEXITSTATUS(status_right);
	else if (WIFSIGNALED(status_right))
		ret = 128 + WTERMSIG(status_right);
	else
		ret = 1;
	return (ret);
}

int	handle_fork_error(int *pipe_fds, pid_t pid_to_wait)
{
	perror("fork");
	close(pipe_fds[0]);
	close(pipe_fds[1]);
	if (pid_to_wait > 0)
		waitpid(pid_to_wait, NULL, 0);
	setup_signals();
	return (1);
}

int	handle_left_builtin(t_ast *left, t_shell *shell)
{
	int	exit_status;

	exit_status = 0;
	if (left->type == NODE_COMMAND && is_state_changing_builtin(left))
	{
		exit_status = exec_builtin(left, shell);
		if (exit_status != 0)
			return (exit_status);
	}
	return (0);
}

static void	print_pipe_output(int status_right)
{
	if (WIFSIGNALED(status_right))
	{
		if (WTERMSIG(status_right) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (WTERMSIG(status_right) == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
	}
	else if (WIFEXITED(status_right))
	{
		if (WEXITSTATUS(status_right) == 130)
			write(STDOUT_FILENO, "\n", 1);
		else if (WEXITSTATUS(status_right) == 131)
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
	}
}
