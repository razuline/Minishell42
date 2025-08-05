/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: preltien <preltien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 12:14:46 by preltien          #+#    #+#             */
/*   Updated: 2025/08/04 17:08:39 by preltien         ###   ########.fr       */
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

static int	execute_and_update(t_pipex_ctx *ctx, pid_t *pid, int *prev_fd,
		t_shell *state)
{
	*pid = fork_and_handle_child(ctx);
	if (*pid == -1)
		return (1);
	pipex_close_fds(prev_fd, ctx->pipe_fd, ctx->is_last);
	if (!ctx->is_last)
		*prev_fd = ctx->pipe_fd[0];
	wait_for_child(*pid, state);
	return (0);
}

int	pipex_exec_loop(t_command *cmds, t_shell *state)
{
	t_pipex_ctx	ctx;
	pid_t		pid;
	int			prev_fd;

	prev_fd = -1;
	while (cmds)
	{
		if (setup_ctx_and_pipe(cmds, &ctx, state, prev_fd))
			return (1);
		if (execute_and_update(&ctx, &pid, &prev_fd, state))
			return (1);
		cmds = cmds->next;
	}
	return (state->exit_code);
}
