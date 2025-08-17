/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 14:47:21 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/17 17:44:44 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	launch_all_children(t_command *cmds, pid_t *pids, t_shell *state);
static void	wait_for_all_children(pid_t *pids, int num_cmds, t_shell *state);

/* Gère l'exécution d'une série de commandes liées par des pipes. */
int	execute_pipeline(t_command *cmds, t_shell *state)
{
	pid_t	*pids;
	int		num_cmds;

	num_cmds = count_commands(cmds);
	pids = malloc(sizeof(pid_t) * num_cmds);
	if (!pids)
	{
		perror("minishell: malloc");
		return (1);
	}
	launch_all_children(cmds, pids, state);
	wait_for_all_children(pids, num_cmds, state);
	free(pids);
	return (state->exit_code);
}

/* Lance tous les processus enfants du pipeline sans attendre. */
static void	launch_all_children(t_command *cmds, pid_t *pids, t_shell *state)
{
	t_exec_context	ctx;
	int				i;
	int				prev_fd;

	i = 0;
	prev_fd = -1;
	while (cmds)
	{
		setup_pipe_context(&ctx, cmds, state, prev_fd);
		pids[i] = fork_and_handle_child(&ctx);
		close_pipe_fds(&prev_fd, ctx.pipe_fd, ctx.is_last);
		if (!ctx.is_last)
			prev_fd = ctx.pipe_fd[0];
		cmds = cmds->next;
		i++;
	}
}

/* Attend la fin de tous les processus enfants du pipeline. */
static void	wait_for_all_children(pid_t *pids, int num_cmds, t_shell *state)
{
	int	i;

	i = 0;
	while (i < num_cmds)
	{
		wait_for_child(pids[i], state);
		i++;
	}
}

/* Logique exécutée par un processus enfant dans un pipeline.
 * Configure les entrées/sorties pour le pipe, puis exécute la commande. */
void	run_pipeline_child(t_exec_context *ctx)
{
	setup_input_output(ctx->prev_fd, ctx->pipe_fd, ctx->is_last);
	run_child_process(ctx->cmd, ctx->state);
}
