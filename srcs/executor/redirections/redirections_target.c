/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_target.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/20 10:00:00 by jose-cad          #+#    #+#             */
/*   Updated: 2026/11/20 10:00:00 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ambiguous_redirect(char *raw)
{
	ft_dprintf(2, "minishell: %s: ambiguous redirect\n", raw);
	return (NULL);
}

static char	*glob_redir_file(char *raw, char *file)
{
	t_list	*matches;
	char	*match;

	matches = get_glob_matches(file);
	if (!matches)
		return (file);
	if (matches->next)
	{
		ft_lstclear(&matches, free);
		free(file);
		return (ambiguous_redirect(raw));
	}
	match = matches->content;
	free(matches);
	free(file);
	return (match);
}

char	*resolve_redir_file(t_ast *node, t_shell *shell)
{
	t_words	words;
	char	*file;

	ft_bzero(&words, sizeof(t_words));
	if (expand_one(node->filename, &words, shell) != 0)
		return (free_words(&words), NULL);
	if (words.len != 1)
	{
		free_words(&words);
		return (ambiguous_redirect(node->filename));
	}
	file = words.argv[0];
	if (!words.quoted[0] && ft_strchr(file, '*'))
		file = glob_redir_file(node->filename, file);
	free(words.argv);
	free(words.quoted);
	return (file);
}
