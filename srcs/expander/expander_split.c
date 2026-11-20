/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 13:02:27 by jose-cad          #+#    #+#             */
/*   Updated: 2025/12/01 13:02:32 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	split_step(t_split *s, char *word, int *i, int *quotes);
static void	split_variable(t_split *s, char *word, int *i, int in_dquote);
static void	split_value(t_split *s, char *value);
static void	flush_word(t_split *s);

int	expand_one(char *word, t_words *out, t_shell *shell)
{
	t_split	s;
	int		i;
	int		quotes[2];

	if (split_init(&s, out, shell, ft_strlen(word) + 1) != 0)
		return (1);
	i = 0;
	quotes[0] = 0;
	quotes[1] = 0;
	while (word[i] && s.buf.str)
		split_step(&s, word, &i, quotes);
	if (s.buf.str && s.has_word)
		flush_word(&s);
	if (!s.buf.str)
		return (1);
	free(s.buf.str);
	return (0);
}

static void	split_step(t_split *s, char *word, int *i, int *quotes)
{
	if (process_quote_state(word[*i], &quotes[0], &quotes[1]))
	{
		s->has_word = 1;
		s->quoted = 1;
		(*i)++;
	}
	else if (word[*i] == '$' && !quotes[0] && (word[*i + 1] == '?'
			|| ft_isalnum_or_underscore(word[*i + 1])))
		split_variable(s, word, i, quotes[1]);
	else
	{
		s->has_word = 1;
		append_literal(word, i, &s->buf);
	}
}

static void	split_variable(t_split *s, char *word, int *i, int in_dquote)
{
	char	*value;

	value = expand_variable(word, i, s->shell);
	if (!value)
	{
		free(s->buf.str);
		s->buf.str = NULL;
		return ;
	}
	if (in_dquote)
		buf_append(&s->buf, value, ft_strlen(value));
	else
		split_value(s, value);
	free(value);
}

static void	split_value(t_split *s, char *value)
{
	int	start;
	int	j;

	j = 0;
	while (value[j] && s->buf.str)
	{
		if (ft_strchr(" \t\n", value[j]))
		{
			if (s->has_word)
				flush_word(s);
			j++;
		}
		else
		{
			start = j;
			while (value[j] && !ft_strchr(" \t\n", value[j]))
				j++;
			buf_append(&s->buf, value + start, j - start);
			s->has_word = 1;
		}
	}
}

static void	flush_word(t_split *s)
{
	if (words_push(s->out, s->buf.str, s->quoted) != 0)
	{
		s->buf.str = NULL;
		return ;
	}
	if (split_init(s, s->out, s->shell, 16) != 0)
		s->buf.str = NULL;
}
