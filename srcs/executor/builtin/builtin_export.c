/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 15:57:44 by jose-cad          #+#    #+#             */
/*   Updated: 2025/12/01 15:57:47 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int			create_new_var(t_shell *shell, char *key, char *value);
static int			update_existing_var(t_env_var *var, char *value);
static t_env_var	*find_env_var(t_shell *shell, const char *key);

int	set_env_var(t_shell *shell, char *key, char *value)
{
	t_env_var	*var;

	var = find_env_var(shell, key);
	if (var && value)
		return (update_existing_var(var, value));
	if (!var)
		return (create_new_var(shell, key, value));
	return (0);
}

int	print_all_env(t_shell *shell)
{
	t_dlist		*lst;
	t_env_var	*var;

	lst = shell->env_list;
	while (lst)
	{
		var = lst->content;
		if (var->value)
			ft_printf("declare -x %s=\"%s\"\n", var->key, var->value);
		else
			ft_printf("declare -x %s\n", var->key);
		lst = lst->next;
	}
	return (0);
}

static t_env_var	*find_env_var(t_shell *shell, const char *key)
{
	t_dlist		*lst;
	t_env_var	*var;

	lst = shell->env_list;
	while (lst)
	{
		var = lst->content;
		if (ft_strcmp(var->key, key) == 0)
			return (var);
		lst = lst->next;
	}
	return (NULL);
}

static int	update_existing_var(t_env_var *var, char *value)
{
	char	*new_value;

	new_value = ft_strdup(value);
	if (!new_value)
		return (1);
	free(var->value);
	var->value = new_value;
	return (0);
}

static int	create_new_var(t_shell *shell, char *key, char *value)
{
	t_env_var	*new;
	t_dlist		*node;

	new = malloc(sizeof(t_env_var));
	if (!new)
		return (1);
	new->key = ft_strdup(key);
	new->value = NULL;
	if (value)
		new->value = ft_strdup(value);
	node = NULL;
	if (new->key && (!value || new->value))
		node = ft_dlstnew(new);
	if (!node)
		return (free_env_var(new), 1);
	ft_dlstadd_back(&shell->env_list, node);
	return (0);
}
