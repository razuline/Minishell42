/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_free_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 12:28:46 by erazumov          #+#    #+#             */
/*   Updated: 2025/06/26 14:48:33 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_redir(t_redir *redir_head);

void	free_commands(t_command *cmd_head)
{
	t_command	*curr;
	t_command	*next;

	curr = cmd_head;
	while (curr != NULL)
	{
		next = curr->next;
		free_redir(curr->redir);
		free(curr->argv);
		free(curr);
		curr = next;
	}
}

static void	free_redir(t_redir *redir_head)
{
	t_redir	*curr;
	t_redir	*next;

	curr = redir_head;
	while (curr != NULL)
	{
		next = curr->next;
		free(curr->file);
		free(curr);
		curr = next;
	}
}
