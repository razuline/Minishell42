/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_free_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 12:28:46 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/06 20:22:38 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_argv(char **argv);
static void	free_redir(t_redir *redir_head);

/* Libère la mémoire d'une liste chaînée de commandes et de leur contenu. */
void	free_commands(t_command *cmd_head)
{
	t_command	*curr;
	t_command	*next;

	curr = cmd_head;
	while (curr != NULL)
	{
		next = curr->next;
		free_redir(curr->redir);
		free_argv(curr->argv);
		free(curr);
		curr = next;
	}
}

/* Libère un tableau d'arguments (argv) et toutes ses chaînes. */
static void	free_argv(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

/* Libère la mémoire d'une liste chaînée de redirections. */
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
