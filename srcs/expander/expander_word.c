/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 16:01:06 by jose-cad          #+#    #+#             */
/*   Updated: 2025/12/01 16:01:08 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_variable_expansion(char *word, int *i,
				t_shell *shell, t_buf *buf);

char	*expand_word(char *word, t_shell *shell)
{
	t_buf	buf;
	int		i;
	int		in_squote;
	int		in_dquote;

	buf.len = 0;
	buf.cap = ft_strlen(word) + 1;
	buf.str = malloc(buf.cap);
	if (!buf.str)
		return (NULL);
	buf.str[0] = '\0';
	i = 0;
	in_squote = 0;
	in_dquote = 0;
	while (word[i] && buf.str)
	{
		if (process_quote_state(word[i], &in_squote, &in_dquote))
			i++;
		else if (word[i] == '$' && !in_squote && (word[i + 1] == '?'
				|| ft_isalnum_or_underscore(word[i + 1])))
			handle_variable_expansion(word, &i, shell, &buf);
		else
			append_literal(word, &i, &buf);
	}
	return (buf.str);
}

static void	handle_variable_expansion(char *word, int *i,
				t_shell *shell, t_buf *buf)
{
	char	*var_value;

	var_value = expand_variable(word, i, shell);
	if (!var_value)
	{
		free(buf->str);
		buf->str = NULL;
		return ;
	}
	buf_append(buf, var_value, ft_strlen(var_value));
	free(var_value);
}

void	append_literal(char *word, int *i, t_buf *buf)
{
	int	start;

	start = *i;
	(*i)++;
	while (word[*i] && word[*i] != '\'' && word[*i] != '"' && word[*i] != '$')
		(*i)++;
	buf_append(buf, word + start, *i - start);
}

char	*expand_heredoc_line(char *line, t_shell *shell)
{
	t_buf	buf;
	int		i;

	buf.len = 0;
	buf.cap = ft_strlen(line) + 1;
	buf.str = malloc(buf.cap);
	if (!buf.str)
		return (NULL);
	buf.str[0] = '\0';
	i = 0;
	while (line[i] && buf.str)
	{
		if (line[i] == '$' && (line[i + 1] == '?'
				|| ft_isalnum_or_underscore(line[i + 1])))
			handle_variable_expansion(line, &i, shell, &buf);
		else
			append_literal(line, &i, &buf);
	}
	return (buf.str);
}

int	process_quote_state(char c, int *in_squote, int *in_dquote)
{
	if (c == '\'')
	{
		if (!*in_dquote)
		{
			*in_squote = !*in_squote;
			return (1);
		}
		return (0);
	}
	if (c == '"')
	{
		if (!*in_squote)
		{
			*in_dquote = !*in_dquote;
			return (1);
		}
		return (0);
	}
	return (0);
}
