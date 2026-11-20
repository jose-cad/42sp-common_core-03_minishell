/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_subshell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 16:02:18 by jose-cad          #+#    #+#             */
/*   Updated: 2025/12/01 16:02:20 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_ast	*parse_subshell_redirs(t_dlist **tokens, t_ast *body)
{
	t_ast	*node;
	t_token	*token;

	node = ast_new_node(NODE_SUBSHELL, body, NULL);
	if (!node)
		return (ast_free(body), NULL);
	while (*tokens)
	{
		token = (*tokens)->content;
		if (token->type < TOKEN_REDIR_IN || token->type > TOKEN_REDIR_ERR)
			break ;
		if (handle_redirection_token(tokens, &node) == -1)
			return (ast_free(node), NULL);
	}
	return (node);
}

t_ast	*parse_subshell(t_dlist **tokens)
{
	t_token	*token;
	t_ast	*node;

	if (!*tokens)
		return (NULL);
	token = (*tokens)->content;
	if (token->type == TOKEN_LPAREN)
	{
		*tokens = (*tokens)->next;
		node = parse_and_or(tokens);
		if (!node)
			return (NULL);
		if (!*tokens || ((t_token *)(*tokens)->content)->type != TOKEN_RPAREN)
		{
			ast_free(node);
			return (NULL);
		}
		*tokens = (*tokens)->next;
		return (parse_subshell_redirs(tokens, node));
	}
	else
		return (parse_command(tokens));
}
