/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 12:14:46 by preltien          #+#    #+#             */
/*   Updated: 2025/08/09 14:13:52 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipex_close_fds(int *prev_fd, int pipe_fd[2], int is_last)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (!is_last)
		close(pipe_fd[1]);
}

static void	setup_input_output(int prev_fd, int pipe_fd[2], int is_last)
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (!is_last)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
}

static void	exit_with_redirection_error(void)
{
	fprintf(stderr, "[ERROR] Failed to apply redirections\n");
	exit(EXIT_FAILURE);
}

static void	exec_command_or_builtin(t_command *cmd, t_shell *state)
{
	if (apply_redirections(cmd->redir) < 0)
		exit_with_redirection_error();
	if (is_builtin(cmd->argv[0]))
		exit(exec_builtin(cmd->argv, state));
	get_absolute_path(cmd->argv, state);
	if (!cmd->argv[0])
	{
		fprintf(stderr, "minishell: command not found\n");
		exit(127);
	}
	execve(cmd->argv[0], cmd->argv, state->envp);
	fprintf(stderr, "minishell: %s: command not found\n", cmd->argv[0]);
	exit(127);
}

void	pipex_fork_and_exec(t_pipex_ctx *ctx)
{
	setup_input_output(ctx->prev_fd, ctx->pipe_fd, ctx->is_last);
	exec_command_or_builtin(ctx->cmd, ctx->state);
}
