/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 16:02:05 by jose-cad          #+#    #+#             */
/*   Updated: 2025/12/01 16:02:06 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_ast	*parse_redirection(t_dlist **tokens, t_token *redir_token);
static int		process_next_command_token(t_dlist **tokens,
					t_ast **cmd_node_ptr, int *argc);

t_ast	*parse_command(t_dlist **tokens)
{
	t_ast	*cmd_node;
	int		status;
	int		argc;

	cmd_node = ast_new_node(NODE_COMMAND, NULL, NULL);
	if (!cmd_node || alloc_argv(cmd_node, *tokens) == -1)
		return (ast_free(cmd_node), NULL);
	argc = 0;
	while (*tokens)
	{
		status = process_next_command_token(tokens, &cmd_node, &argc);
		if (status == -1)
			return (ast_free(cmd_node), NULL);
		if (status == 0)
			break ;
	}
	if (!cmd_node->argv && !cmd_node->filename)
	{
		ast_free(cmd_node);
		return (NULL);
	}
	return (cmd_node);
}

int	handle_redirection_token(t_dlist **tokens, t_ast **cmd_node_ptr)
{
	t_ast	*redir_node;
	t_token	*redir_token;

	redir_token = (*tokens)->content;
	*tokens = (*tokens)->next;
	redir_node = parse_redirection(tokens, redir_token);
	if (!redir_node)
		return (-1);
	redir_node->left = *cmd_node_ptr;
	*cmd_node_ptr = redir_node;
	return (1);
}

static int	process_next_command_token(t_dlist **tokens, t_ast **cmd_node_ptr,
		int *argc)
{
	t_token	*token;
	t_ast	*actual_cmd;

	token = (*tokens)->content;
	if (token->type == TOKEN_WORD)
	{
		actual_cmd = get_command_node(*cmd_node_ptr);
		if (!actual_cmd)
			actual_cmd = *cmd_node_ptr;
		actual_cmd->argv[*argc] = ft_strdup(token->value);
		if (!actual_cmd->argv[*argc])
			return (-1);
		actual_cmd->argv_quoted[*argc] = token->was_quoted;
		(*argc)++;
		*tokens = (*tokens)->next;
		return (1);
	}
	else if (token->type >= TOKEN_REDIR_IN && token->type <= TOKEN_REDIR_ERR)
		return (handle_redirection_token(tokens, cmd_node_ptr));
	else
		return (0);
}

static t_ast	*parse_redirection(t_dlist **tokens, t_token *redir_token)
{
	t_ast		*redir_node;
	t_token		*file_token;
	char		*processed_filename;

	if (!(*tokens) || ((t_token *)(*tokens)->content)->type != TOKEN_WORD)
		return (NULL);
	file_token = (*tokens)->content;
	*tokens = (*tokens)->next;
	if (redir_token->type == TOKEN_HEREDOC)
		processed_filename = process_filename(file_token->value);
	else
		processed_filename = ft_strdup(file_token->value);
	if (!processed_filename)
		return (NULL);
	redir_node = create_redir_node(redir_token->type);
	if (!redir_node)
	{
		free(processed_filename);
		return (NULL);
	}
	if (redir_token->type == TOKEN_HEREDOC && file_token->was_quoted)
		redir_node->heredoc_expand = 0;
	redir_node->filename = processed_filename;
	return (redir_node);
}
