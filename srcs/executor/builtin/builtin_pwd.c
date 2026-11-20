/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 15:57:51 by jose-cad          #+#    #+#             */
/*   Updated: 2025/12/01 15:57:53 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(t_shell *shell)
{
	char	cwd[4096];
	char	*pwd_env;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		pwd_env = get_env_value("PWD", shell);
		if (pwd_env)
		{
			ft_putendl_fd(pwd_env, 1);
			return (0);
		}
		perror("pwd");
		return (1);
	}
	ft_putendl_fd(cwd, 1);
	return (0);
}
