/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_glob_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 16:03:27 by jose-cad          #+#    #+#             */
/*   Updated: 2025/12/01 16:03:28 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	insert_sorted(t_list **lst, char *name);
static int	only_stars(const char *pattern);
static int	glob_match_from(const char *pattern, const char *text,
				const char *star, const char *mark);

int	is_glob_match(const char *pattern, const char *text)
{
	return (glob_match_from(pattern, text, NULL, NULL));
}

static int	glob_match_from(const char *pattern, const char *text,
		const char *star, const char *mark)
{
	while (*text)
	{
		if (*pattern == '*')
		{
			star = pattern++;
			mark = text;
		}
		else if (*pattern == *text)
		{
			pattern++;
			text++;
		}
		else if (!star)
			return (0);
		else
		{
			pattern = star + 1;
			text = ++mark;
		}
	}
	return (only_stars(pattern));
}

static int	only_stars(const char *pattern)
{
	while (*pattern == '*')
		pattern++;
	return (*pattern == '\0');
}

t_list	*get_glob_matches(char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	t_list			*matches;

	matches = NULL;
	dir = opendir(".");
	if (!dir)
		return (NULL);
	while (1)
	{
		entry = readdir(dir);
		if (!entry)
			break ;
		if (entry->d_name[0] == '.' && pattern[0] != '.')
			continue ;
		if (is_glob_match(pattern, entry->d_name))
			insert_sorted(&matches, entry->d_name);
	}
	closedir(dir);
	return (matches);
}

static void	insert_sorted(t_list **lst, char *name)
{
	t_list	*new;
	char	*copy;

	copy = ft_strdup(name);
	if (!copy)
		return ;
	new = ft_lstnew(copy);
	if (!new)
	{
		free(copy);
		return ;
	}
	while (*lst && ft_strcmp((*lst)->content, new->content) < 0)
		lst = &(*lst)->next;
	new->next = *lst;
	*lst = new;
}
