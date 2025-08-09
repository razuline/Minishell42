/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 13:04:53 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/09 17:24:45 by erazumov         ###   ########.fr       */
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

/* Ajoute une nouvelle chaîne à un tableau d'arguments (argv),
	en le réallouant. */
char	**create_argv(char **old_argv, char *new_str)
{
	int		count;
	char	**new_argv;

	count = count_argv_elements(old_argv);
	new_argv = malloc(sizeof(char *) * (count + 2));
	if (!new_argv)
	{
		printf("[ERROR] create_argv: malloc failed\n");
		return (NULL);
	}
	copy_old_argv(new_argv, old_argv, count);
	new_argv[count] = strdup(new_str);
	if (!new_argv[count])
	{
		printf("[ERROR] create_argv: strdup failed for '%s'\n", new_str);
		free(new_argv);
		return (NULL);
	}
	new_argv[count + 1] = NULL;
	free(old_argv);
	return (new_argv);
}

/* Compte le nombre d'éléments dans un tableau argv. */
static int	count_argv_elements(char **argv)
{
	int	count;

	count = 0;
	if (argv != NULL)
	{
		while (argv[count] != NULL)
			count++;
	}
	return (count);
}

/* Copie les pointeurs de l'ancien tableau vers le nouveau. */
static void	copy_old_argv(char **new_argv, char **old_argv, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		new_argv[i] = old_argv[i];
		i++;
	}
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
