/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_argv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 13:02:27 by jose-cad          #+#    #+#             */
/*   Updated: 2025/12/01 13:02:32 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_assignment_arg(t_ast *cmd, int i);
static int	expand_token(t_ast *cmd, int i, t_words *out, t_shell *shell);

int	expand_argv(t_ast *cmd, t_shell *shell)
{
	t_words	out;
	int		i;

	ft_bzero(&out, sizeof(t_words));
	i = 0;
	while (cmd->argv[i])
	{
		if (expand_token(cmd, i, &out, shell) != 0)
			return (free_words(&out), 1);
		i++;
	}
	free_char_array(cmd->argv);
	free(cmd->argv_quoted);
	cmd->argv = out.argv;
	cmd->argv_quoted = out.quoted;
	return (0);
}

static int	is_assignment_arg(t_ast *cmd, int i)
{
	if (!ft_strchr(cmd->argv[i], '=') || !validate_key(cmd->argv[i]))
		return (0);
	return (i == 0 || ft_strcmp(cmd->argv[0], "export") == 0);
}

static int	expand_token(t_ast *cmd, int i, t_words *out, t_shell *shell)
{
	char	*word;

	if (!is_assignment_arg(cmd, i))
		return (expand_one(cmd->argv[i], out, shell));
	word = expand_word(cmd->argv[i], shell);
	if (!word)
		return (1);
	return (words_push(out, word, cmd->argv_quoted[i]));
}

int	split_init(t_split *s, t_words *out, t_shell *shell, size_t cap)
{
	s->out = out;
	s->shell = shell;
	s->has_word = 0;
	s->quoted = 0;
	s->buf.len = 0;
	s->buf.cap = cap;
	s->buf.str = malloc(cap);
	if (!s->buf.str)
		return (1);
	s->buf.str[0] = '\0';
	return (0);
}

void	free_words(t_words *words)
{
	if (words->argv)
		free_char_array(words->argv);
	free(words->quoted);
	words->argv = NULL;
	words->quoted = NULL;
	words->len = 0;
	words->cap = 0;
}
