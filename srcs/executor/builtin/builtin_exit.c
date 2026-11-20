/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 15:57:35 by jose-cad          #+#    #+#             */
/*   Updated: 2025/12/01 15:57:37 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>

static void	exit_with_error(char *arg, t_shell *shell, t_ast *node);
static int	parse_exit_code(const char *s, long long *code);

int	builtin_exit(t_ast *node, t_shell *shell)
{
	long long	code;

	if (!shell->is_child && isatty(STDIN_FILENO))
		ft_putendl_fd("exit", 2);
	code = shell->exit_status;
	if (node->argv[1] && !parse_exit_code(node->argv[1], &code))
		exit_with_error(node->argv[1], shell, node);
	if (node->argv[1] && node->argv[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		return (1);
	}
	clean_exit_child(shell, (unsigned char)code);
	return (0);
}

static void	exit_with_error(char *arg, t_shell *shell, t_ast *node)
{
	(void)node;
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(": numeric argument required", 2);
	clean_exit_child(shell, 2);
}

static int	parse_exit_code(const char *s, long long *code)
{
	unsigned long long	n;
	unsigned long long	limit;
	int					neg;

	n = 0;
	while (ft_isspace(*s))
		s++;
	neg = (*s == '-');
	if (*s == '-' || *s == '+')
		s++;
	limit = (unsigned long long)LLONG_MAX + neg;
	if (!ft_isdigit(*s))
		return (0);
	while (ft_isdigit(*s))
	{
		if (n > (limit - (*s - '0')) / 10)
			return (0);
		n = n * 10 + (*s++ - '0');
	}
	if (*s)
		return (0);
	if (neg)
		n = 0 - n;
	*code = (unsigned char)n;
	return (1);
}
