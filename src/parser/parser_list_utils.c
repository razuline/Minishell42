/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_list_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:22:40 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/10 15:29:44 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Alloue et initialise une nouvelle structure de commande (t_command). */
t_command	*create_command(void)
{
	t_command	*new_cmd;

	new_cmd = malloc(sizeof(t_command));
	if (!new_cmd)
		return (NULL);
	new_cmd->argv = NULL;
	new_cmd->redir = NULL;
	new_cmd->next = NULL;
	return (new_cmd);
}

/* Ajoute une structure de redirection à la liste d'une commande. */
void	add_redir_to_cmd(t_command *cmd, t_redir *new_redir)
{
	t_redir	*curr;

	if (cmd->redir == NULL)
		cmd->redir = new_redir;
	else
	{
		curr = cmd->redir;
		while (curr->next != NULL)
			curr = curr->next;
		curr->next = new_redir;
	}
}

/* Compte le nombre de commandes dans une liste chaînée. */
int	count_commands(t_command *cmds)
{
	int	count;

	count = 0;
	while (cmds)
	{
		count++;
		cmds = cmds->next;
	}
	return (count);
}

/* Vérifie si une liste de commandes contient un pipe (plus d'une commande). */
int	has_pipe(t_command *cmds)
{
	return (cmds && cmds->next != NULL);
}
