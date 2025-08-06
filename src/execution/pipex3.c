/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: preltien <preltien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 12:14:46 by preltien          #+#    #+#             */
/*   Updated: 2025/08/06 12:50:42 by preltien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static int	setup_ctx_and_pipe(t_command *cmd, t_pipex_ctx *ctx, t_shell *state,
		int prev_fd)
{
	ctx->cmd = cmd;
	ctx->state = state;
	ctx->prev_fd = prev_fd;
	ctx->is_last = (cmd->next == NULL);
	if (create_pipe_if_needed(ctx->pipe_fd, ctx->is_last))
		return (1);
	return (0);
}

int	pipex_exec_loop(t_command *cmds, t_shell *state)
{
	t_pipex_ctx	ctx;
	int			pid_count;
	int			prev_fd;
	int			ret;
	pid_t		pid;
	int			status;
	pid_t		pids[100];

	pid_count = 0;
	prev_fd = -1;
	while (cmds)
	{
		ret = setup_ctx_and_pipe(cmds, &ctx, state, prev_fd);
		if (ret)
			return (1);
		pid = fork_and_handle_child(&ctx);
		if (pid == -1)
			return (1);
		pipex_close_fds(&prev_fd, ctx.pipe_fd, ctx.is_last);
		if (!ctx.is_last)
			prev_fd = ctx.pipe_fd[0];
		pids[pid_count++] = pid;
		cmds = cmds->next;
	}
	for (int i = 0; i < pid_count; i++)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			state->exit_code = WEXITSTATUS(status);
		else
			state->exit_code = 1;
	}
	return (state->exit_code);
}
