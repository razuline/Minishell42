/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: preltien <preltien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:33:07 by preltien          #+#    #+#             */
/*   Updated: 2025/08/07 10:32:07 by preltien         ###   ########.fr       */
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
 * Exécute une commande externe via fork + execve
 */
static void	child_process(t_command *cmd, t_shell *state)
{
	if (apply_redirections(cmd->redir) < 0)
	{
		fprintf(stderr, "[ERROR] Failed to apply redirections\n");
		exit(EXIT_FAILURE);
	}
	if (is_directory(cmd->argv[0]))
	{
		fprintf(stderr, "minishell: %s: Is a directory\n", cmd->argv[0]);
		exit(126);
	}
	if (access(cmd->argv[0], F_OK) != 0)
	{
		fprintf(stderr, "minishell: %s: No such file or directory\n",
			cmd->argv[0]);
		exit(127);
	}
	if (access(cmd->argv[0], X_OK) != 0)
	{
		fprintf(stderr, "minishell: %s: Permission denied\n", cmd->argv[0]);
		exit(126);
	}
	execve(cmd->argv[0], cmd->argv, state->envp);
	fprintf(stderr, "minishell: %s: command not found\n", cmd->argv[0]);
	exit(127);
}

static int	parent_process(pid_t pid, t_shell *state)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		state->exit_code = WEXITSTATUS(status);
	else
		state->exit_code = 1;
	return (state->exit_code);
}

int	exec_cmd(t_command *cmd, t_shell *state)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		state->exit_code = 1;
		return (1);
	}
	if (pid == 0)
		child_process(cmd, state);
	return (parent_process(pid, state));
}
