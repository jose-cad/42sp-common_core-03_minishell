/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_argv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 16:01:56 by jose-cad          #+#    #+#             */
/*   Updated: 2025/12/01 16:01:58 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_words(t_dlist *tokens);

int	alloc_argv(t_ast *cmd, t_dlist *tokens)
{
	int	count;

	count = count_words(tokens);
	if (count == 0)
		return (0);
	cmd->argv = ft_calloc(count + 1, sizeof(char *));
	cmd->argv_quoted = ft_calloc(count + 1, sizeof(int));
	if (!cmd->argv || !cmd->argv_quoted)
		return (-1);
	return (0);
}

static int	count_words(t_dlist *tokens)
{
	int		count;
	t_token	*token;

	count = 0;
	while (tokens)
	{
		token = tokens->content;
		if (token->type >= TOKEN_REDIR_IN && token->type <= TOKEN_REDIR_ERR)
		{
			tokens = tokens->next;
			if (tokens)
				tokens = tokens->next;
			continue ;
		}
		if (token->type != TOKEN_WORD)
			break ;
		count++;
		tokens = tokens->next;
	}
	return (count);
}
