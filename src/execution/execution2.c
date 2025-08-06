/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:33:07 by preltien          #+#    #+#             */
/*   Updated: 2025/08/06 18:49:19 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

extern char	**environ;

/*
 * Parcourt la liste des commandes et les exécute
 */
static int	execute_builtin_cmd(t_command *cmd, t_shell *state)
{
	int	saved_stdin;
	int	saved_stdout;
	int	ret;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (apply_redirections(cmd->redir) < 0)
	{
		fprintf(stderr, "[ERROR] Failed to apply redirections\n");
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return (1);
	}
	ret = exec_builtin(cmd->argv, state);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	state->exit_code = ret;
	return (ret);
}

static int	execute_external_cmd(t_command *cmd, t_shell *state)
{
	int	ret;

	get_absolute_path(cmd->argv, state);
	if (cmd->argv[0])
		ret = exec_cmd(cmd, state);
	else
	{
		fprintf(stderr, "minishell: command not found: %s\n", cmd->argv[0]);
		ret = 127;
	}
	state->exit_code = ret;
	return (ret);
}

int	execute(t_command *cmds, t_shell *state)
{
	t_command	*cmd;
	int			ret;

	if (!cmds)
	{
		fprintf(stderr, "[ERROR] cmd is NULL in execute\n");
		return (1);
	}
	if (has_pipe(cmds))
		return (pipex_exec_loop(cmds, state));
	cmd = cmds;
	if (!cmd->argv || !cmd->argv[0])
	{
		fprintf(stderr, "[WARNING] Empty command, skipping...\n");
		return (0);
	}
	if (is_builtin(cmd->argv[0]))
		ret = execute_builtin_cmd(cmd, state);
	else
		ret = execute_external_cmd(cmd, state);
	fflush(stdout);
	return (ret);
}
