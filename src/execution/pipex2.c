/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: preltien <preltien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 12:14:46 by preltien          #+#    #+#             */
/*   Updated: 2025/08/05 12:51:36 by preltien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int	has_pipe(t_command *cmds)
{
	t_command	*current;

	current = cmds;
	while (current)
	{
		if (current->next)
			return (1);
		current = current->next;
	}
	return (0);
}

int	create_pipe_if_needed(int pipe_fd[2], int is_last)
{
	if (!is_last && pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (1);
	}
	return (0);
}

int	fork_and_handle_child(t_pipex_ctx *ctx)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
		pipex_fork_and_exec(ctx);
	return (pid);
}

void	wait_for_child(pid_t pid, t_shell *state)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		state->exit_code = WEXITSTATUS(status);
	else
		state->exit_code = 1;
}
