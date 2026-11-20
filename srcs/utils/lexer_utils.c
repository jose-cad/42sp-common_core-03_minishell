/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 16:03:39 by jose-cad          #+#    #+#             */
/*   Updated: 2025/12/01 16:03:40 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_spaces(char *line, int *i)
{
	while (line[*i] && (line[*i] == ' ' || line[*i] == '\t'))
		(*i)++;
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

int	is_operator(char *line, int i)
{
	if (line[i] == '|' || line[i] == '<' || line[i] == '>'
		|| line[i] == '(' || line[i] == ')' || line[i] == '&')
		return (1);
	if (line[i] == '2' && line[i + 1] == '>' && (i == 0
			|| ft_isspace(line[i - 1]) || ft_strchr("|<>()&", line[i - 1])))
		return (1);
	return (0);
}

char	*extract_word(char *line, int *i)
{
	int		start;
	int		len;
	char	*word;

	start = *i;
	while (line[*i] && !is_quote(line[*i]) && !is_operator(line, *i)
		&& !ft_isspace(line[*i]))
		(*i)++;
	len = *i - start;
	word = dup_slice(line + start, len);
	return (word);
}

char	*dup_slice(const char *s, size_t len)
{
	char	*dup;

	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	ft_memcpy(dup, s, len);
	dup[len] = '\0';
	return (dup);
}
