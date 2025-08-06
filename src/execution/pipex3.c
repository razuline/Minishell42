/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 12:14:46 by preltien          #+#    #+#             */
/*   Updated: 2025/08/06 19:30:23 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static int	setup_pipe_context(t_command *cmd, t_pipex_ctx *ctx, t_shell *state,
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
	pid_t	*pids;
	int		num_cmds;

	num_cmds = count_commands(cmds);
	pids = malloc(sizeof(pid_t) * num_cmds);
	if (!pids)
		return (1);
	launch_all_processes(cmds, pids, state);
	wait_for_all_processes(pids, num_cmds, state);
	free(pids);
	return (state->exit_code);
}

static void	launch_all_processes(t_command *cmds, pid_t *pids, t_shell *state)
{
	t_pipex_ctx	ctx;
	int			i;
	int			prev_fd;

	i = 0;
	prev_fd = -1;
	while (cmds)
	{
		setup_pipe_context(cmds, &ctx, state, prev_fd);
		pids[i] = fork_and_handle_child(&ctx);
		pipex_close_fds(&prev_fd, ctx.pipe_fd, ctx.is_last);
		if (!ctx.is_last)
			prev_fd = ctx.pipe_fd[0];
		cmds = cmds->next;
		i++;
	}
}

static void	wait_for_all_processes(pid_t *pids, int num_cmds, t_shell *state)
{
	int	i;

	i = 0;
	while (i < num_cmds)
	{
		wait_for_child(pids[i], state);
		i++;
	}
}
