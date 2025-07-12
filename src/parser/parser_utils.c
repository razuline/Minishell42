/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 13:04:53 by erazumov          #+#    #+#             */
/*   Updated: 2025/06/25 12:30:58 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	**create_argv(char **old_argv, char *new_str)
{
	int		i;
	int		count;
	char	**new_argv;

	count = 0;
	if (old_argv != NULL)
	{
		while (old_argv[count] != NULL)
			count++;
	}
	new_argv = malloc(sizeof(char *) * (count + 2));
	if (!new_argv)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_argv[i] = old_argv[i];
		i++;
	}
	new_argv[count] = new_str;
	new_argv[count + 1] = NULL;
	free(old_argv);
	return (new_argv);
}

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
