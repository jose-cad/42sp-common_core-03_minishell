/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@42sp.org.br>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 13:02:38 by jose-cad          #+#    #+#             */
/*   Updated: 2025/12/01 13:02:42 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	env_array_cleanup(char **envp, int i);
static int	count_env_values(t_dlist *env_list);
static int	convert_node_to_string(t_dlist *curr, char **envp, int i);

char	**env_list_to_array(t_dlist *env_list)
{
	char	**envp;
	t_dlist	*curr;
	int		count;
	int		i;

	count = count_env_values(env_list);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	curr = env_list;
	i = 0;
	while (curr)
	{
		if (((t_env_var *)curr->content)->value)
		{
			if (convert_node_to_string(curr, envp, i) != 0)
				return (NULL);
			i++;
		}
		curr = curr->next;
	}
	envp[i] = NULL;
	return (envp);
}

static int	count_env_values(t_dlist *env_list)
{
	int	count;

	count = 0;
	while (env_list)
	{
		if (((t_env_var *)env_list->content)->value)
			count++;
		env_list = env_list->next;
	}
	return (count);
}

static void	env_array_cleanup(char **envp, int i)
{
	while (i > 0)
		free(envp[--i]);
	free(envp);
}

static int	convert_node_to_string(t_dlist *curr, char **envp, int i)
{
	t_env_var	*var;
	char		*temp;

	var = curr->content;
	temp = ft_strjoin3(var->key, "=", var->value);
	if (!temp)
	{
		env_array_cleanup(envp, i);
		return (1);
	}
	envp[i] = temp;
	return (0);
}
