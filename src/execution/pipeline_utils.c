/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 14:56:12 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/10 15:34:24 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Prépare le contexte d'exécution pour une commande dans le pipeline. */
void	setup_pipe_context(t_exec_context *ctx, t_command *cmd, t_shell *state,
		int prev_fd)
{
	ctx->cmd = cmd;
	ctx->state = state;
	ctx->prev_fd = prev_fd;
	ctx->is_last = (cmd->next == NULL);
	if (create_pipe_if_needed(ctx->pipe_fd, ctx->is_last) != 0)
		exit(EXIT_FAILURE);
}

/* Ferme les descripteurs de fichier du pipe dans le processus parent. */
void	close_pipe_fds(int *prev_fd, int pipe_fd[2], int is_last)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (!is_last)
		close(pipe_fd[1]);
}

/* Crée un pipe sauf pour la dernière commande. */
int	create_pipe_if_needed(int pipe_fd[2], int is_last)
{
	if (!is_last)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("minishell: pipe");
			return (1);
		}
	}
	return (0);
}

/* Configure l'entrée et la sortie d'un processus dans un pipeline. */
void	setup_input_output(int prev_fd, int pipe_fd[2], int is_last)
{
	if (prev_fd != -1)
	{
		if (dup2(prev_fd, STDIN_FILENO) < 0)
		{
			perror("minishell: dup2");
			exit(EXIT_FAILURE);
		}
		close(prev_fd);
	}
	if (!is_last)
	{
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], STDOUT_FILENO) < 0)
		{
			perror("minishell: dup2");
			exit(EXIT_FAILURE);
		}
		close(pipe_fd[1]);
	}
}
