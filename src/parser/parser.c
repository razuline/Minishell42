/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 12:55:09 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/04 15:04:18 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	dispatch_token(t_command **cmd_ptr, t_token **tok_ptr);
static int	process_token(t_command *cmd, t_token **tok_ptr);
static int	process_pipe(t_command **cmd_ptr, t_token **tok_ptr);
static int	process_redir(t_command *cmd, t_token **curr_tok);

t_command	*parser(t_token *token_lst)
{
	t_command	*cmd_lst_head;
	t_command	*curr_cmd;
	t_token		*curr_token;

	if (!token_lst)
		return (NULL);
	cmd_lst_head = create_command();
	curr_cmd = cmd_lst_head;
	curr_token = token_lst;
	while (curr_token != NULL)
	{
		while (curr_token && ft_isspace(curr_token->value[0]))
			curr_token = curr_token->next;
		if (!curr_token)
			break ;
		if (dispatch_token(&curr_cmd, &curr_token) != 0)
		{
			free_commands(cmd_lst_head);
			return (NULL);
		}
	}
	return (cmd_lst_head);
}

static int	dispatch_token(t_command **cmd_ptr, t_token **tok_ptr)
{
	if ((*tok_ptr)->type == WORD)
		return (process_token(*cmd_ptr, tok_ptr));
	else if (ft_redirection((*tok_ptr)->type))
		return (process_redir(*cmd_ptr, tok_ptr));
	else if ((*tok_ptr)->type == PIPE)
		return (process_pipe(cmd_ptr, tok_ptr));
	printf("minishell: syntax error\n");
	return (1);
}

static int	process_token(t_command *cmd, t_token **tok_ptr)
{
	char	*val_cp;

	val_cp = ft_strdup((*tok_ptr)->value);
	if (!val_cp)
		return (1);
	cmd->argv = create_argv(cmd->argv, val_cp);
	if (!cmd->argv)
	{
		free(val_cp);
		return (1);
	}
	*tok_ptr = (*tok_ptr)->next;
	return (0);
}

static int	process_pipe(t_command **cmd_ptr, t_token **tok_ptr)
{
	t_command	*new_cmd;

	if ((*cmd_ptr)->argv == NULL && (*cmd_ptr)->redir == NULL)
	{
		printf("minishell: syntax error near unexpected token '|'\n");
		return (1);
	}
	new_cmd = create_command();
	if (!new_cmd)
		return (1);
	(*cmd_ptr)->next = new_cmd;
	*cmd_ptr = new_cmd;
	*tok_ptr = (*tok_ptr)->next;
	return (0);
}

static int	process_redir(t_command *cmd, t_token **tok_ptr)
{
	t_token	*token_file;
	t_redir	*new_redir;

	token_file = (*tok_ptr)->next;
	if (!token_file || token_file->type != WORD)
	{
		printf("minishell: syntax error near unexpected token\n");
		return (1);
	}
	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (1);
	new_redir->type = (*tok_ptr)->type;
	new_redir->file = ft_strdup(token_file->value);
	new_redir->next = NULL;
	if (!new_redir->file)
	{
		free(new_redir);
		return (1);
	}
	add_redir_to_cmd(cmd, new_redir);
	*tok_ptr = (*tok_ptr)->next->next;
	return (0);
}

/*
** CHANGEMENTS
**
** 4 août :
** process_token() : Utilisation ft_strdup pour créer une copie de la valeur 
** 					du token. C'est cette copie qui est maintenant stockée
**					dans argv.
** process_pipe() : La condition if vérifie maintenant qu'une commande est
**					totalement vide (ni arguments, ni redirections) avant de
**					déclarer une erreur de syntaxe.
*/
