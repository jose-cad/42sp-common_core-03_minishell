/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 16:03:32 by jose-cad          #+#    #+#             */
/*   Updated: 2025/12/01 16:03:34 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_var_name_len(char *str)
{
	int	i;

	if (str[0] == '?')
		return (1);
	i = 0;
	while (str[i] && ft_isalnum_or_underscore(str[i]))
		i++;
	return (i);
}

char	**ft_lst_to_array(t_list *list)
{
	char	**array;
	int		len;
	int		i;
	t_list	*current;

	len = ft_lstsize(list);
	array = (char **)ft_calloc(len + 1, sizeof(char *));
	if (!array)
		return (NULL);
	current = list;
	i = 0;
	while (current)
	{
		array[i] = (char *)current->content;
		current->content = NULL;
		current = current->next;
		i++;
	}
	return (array);
}

int	should_expand_wildcards(char **argv, int *argv_quoted)
{
	int	i;

	if (!argv || !argv_quoted)
		return (0);
	i = 0;
	while (argv[i])
	{
		if (ft_strchr(argv[i], '*') && !argv_quoted[i])
			return (1);
		i++;
	}
	return (0);
}

void	buf_append(t_buf *buf, const char *s, size_t n)
{
	char	*new_str;

	if (!buf->str)
		return ;
	if (buf->len + n + 1 > buf->cap)
	{
		while (buf->len + n + 1 > buf->cap)
			buf->cap *= 2;
		new_str = malloc(buf->cap);
		if (new_str)
			ft_memcpy(new_str, buf->str, buf->len);
		free(buf->str);
		buf->str = new_str;
		if (!new_str)
			return ;
	}
	ft_memcpy(buf->str + buf->len, s, n);
	buf->len += n;
	buf->str[buf->len] = '\0';
}

int	words_push(t_words *words, char *word, int quoted)
{
	char	**new_argv;
	int		*new_quoted;
	int		new_cap;

	if (!word)
		return (1);
	if (words->len + 1 >= words->cap)
	{
		new_cap = words->cap * 2 + 8;
		new_argv = ft_calloc(new_cap, sizeof(char *));
		new_quoted = ft_calloc(new_cap, sizeof(int));
		if (!new_argv || !new_quoted)
			return (free(new_argv), free(new_quoted), free(word), 1);
		ft_memcpy(new_argv, words->argv, words->len * sizeof(char *));
		ft_memcpy(new_quoted, words->quoted, words->len * sizeof(int));
		free(words->argv);
		free(words->quoted);
		words->argv = new_argv;
		words->quoted = new_quoted;
		words->cap = new_cap;
	}
	words->argv[words->len] = word;
	words->quoted[words->len++] = quoted;
	return (0);
}
