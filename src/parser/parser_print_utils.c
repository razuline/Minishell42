/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_print_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 12:28:18 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/06 20:24:29 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_argv(char **argv);
static void	print_redirections(t_redir *redir_lst);

/* Vérifie si un type de token est une redirection. */
int	is_redir_token(int type)
{
	return (type == REDIRECT_IN || type == REDIRECT_OUT || type == APPEND_OUT
		|| type == HEREDOC);
}

/* Affiche le contenu d'une liste de commandes (pour le debug). */
void	print_commands(t_command *cmd_head)
{
	t_command	*curr_cmd;
	int			i;

	curr_cmd = cmd_head;
	i = 1;
	while (curr_cmd != NULL)
	{
		printf("--- Command %d ---\n", i);
		print_argv(curr_cmd->argv);
		print_redirections(curr_cmd->redir);
		curr_cmd = curr_cmd->next;
		i++;
	}
}

/* Affiche le contenu d'un tableau d'arguments (pour le debug). */
static void	print_argv(char **argv)
{
	int	i;

	if (argv == NULL)
	{
		printf("(empty)\n");
		return ;
	}
	i = 0;
	while (argv[i] != NULL)
	{
		printf(" argv[%d]: \"%s\"\n", i, argv[i]);
		i++;
	}
	printf(" argv[%d]: (null)\n", i);
}

/* Affiche le contenu d'une liste de redirections (pour le debug). */
static void	print_redirections(t_redir *redir_lst)
{
	t_redir	*curr;

	if (redir_lst == NULL)
	{
		printf("  Redirections: (none)\n");
		return ;
	}
	printf("  Redirections:\n");
	curr = redir_lst;
	while (curr != NULL)
	{
		printf("    -> TYPE: %s, FILE: \"%s\"\n", get_type_name(curr->type),
				curr->file);
		curr = curr->next;
	}
}
