/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 12:55:09 by erazumov          #+#    #+#             */
/*   Updated: 2025/06/21 18:39:41 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_token(t_command *cmd, t_token *tok);
static int	process_pipe(t_command **cmd_ptr);
static int	process_redir(t_command *cmd, t_token **curr_tok);

t_command	*parser(t_token *token_lst)
{
	t_command	*head_cmd_lst;
	t_command	*curr_cmd;
	t_command	*next_cmd;
	t_token		*curr_token;

	if (!token_lst)
		return (NULL);
	curr_cmd = create_cmd();
	head_cmd_lst = curr_cmd;
	curr_token = token_lst;
	while (curr_token != NULL)
	{
		if (curr_token->type == WORD)
			process_token(curr_cmd, curr_token);
		else if (ft_redirection(curr_token->type))
			process_redir(&curr_cmd, &curr_token);
		else if (curr_token->type == PIPE)
		{
			if (process_pipe(&curr_cmd) != 0)
				return (NULL, free_cmd(head_cmd_lst));
		}
		curr_token = curr_token->next;
	}
	return (head_cmd_lst);
}

static int	process_token(t_command *cmd, t_token *tok)
{
	cmd->argv = create_argv(cmd->argv, tok->value);
}

static int	process_pipe(t_command **cmd_ptr)
{
	t_command	*new_cmd;

	if ((*cmd_ptr)->argv == NULL)
	{
		printf("minishell: syntax error near unexpected token '|'\n");
		return (1);
	}
	new_cmd = create_cmd();
	if (!new_cmd)
		return (1);
	(*cmd_ptr)->next = new_cmd;
	*cmd_ptr = new_cmd;
	return (0);
}

static int	process_redir(t_command *cmd, t_token **curr_tok)
{
	t_token	*token_filename;
	t_redir	*new_redir;

	token_filename = (*curr_tok)->next;
	if (!token_filename || token_filename->type != WORD)
	{
		printf("minishell: syntax error near unexpected token\n");
		return (1);
	}
	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (1);
	new_redir->type = (*curr_tok)->type;
	new_redir->filename = ft_strdup(token_filename->value);
	new_redir->next = NULL;
	if (!new_redir->filename)
	{
		free(new_redir);
		return (1);
	}
	add_redir_to_cmd(cmd, new_redir);
	*curr_tok = (*curr_tok)->next;
	return (0);
}
