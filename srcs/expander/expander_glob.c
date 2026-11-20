/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_glob.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 13:02:27 by jose-cad          #+#    #+#             */
/*   Updated: 2025/12/01 13:02:32 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	finalize_wildcard_expansion(char ***argv_ptr, t_list *list);
static int	process_wildcard_arg(char *arg, int quoted, t_list ***end);

int	expand_wildcards(char ***argv_ptr, int *quoted)
{
	t_list	*new_arg_list;
	t_list	**end;
	int		i;

	new_arg_list = NULL;
	end = &new_arg_list;
	i = 0;
	while ((*argv_ptr)[i])
	{
		if (process_wildcard_arg((*argv_ptr)[i], quoted[i], &end) != 0)
			return (ft_lstclear(&new_arg_list, free), 1);
		i++;
	}
	return (finalize_wildcard_expansion(argv_ptr, new_arg_list));
}

static int	process_wildcard_arg(char *arg, int quoted, t_list ***end)
{
	t_list	*matches;
	char	*copy;

	matches = NULL;
	if (!quoted && ft_strchr(arg, '*') != NULL)
		matches = get_glob_matches(arg);
	if (!matches)
	{
		copy = ft_strdup(arg);
		if (copy)
			matches = ft_lstnew(copy);
		if (!matches)
			return (free(copy), 1);
	}
	**end = matches;
	while (**end)
		*end = &(**end)->next;
	return (0);
}

static int	finalize_wildcard_expansion(char ***argv_ptr, t_list *list)
{
	t_list	*temp;
	char	**new_argv;

	new_argv = ft_lst_to_array(list);
	if (!new_argv)
		return (ft_lstclear(&list, free), 1);
	free_char_array(*argv_ptr);
	*argv_ptr = new_argv;
	while (list)
	{
		temp = list;
		list = list->next;
		free(temp);
	}
	return (0);
}
