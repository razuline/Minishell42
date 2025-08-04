/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: preltien <preltien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 12:14:46 by preltien          #+#    #+#             */
/*   Updated: 2025/08/04 14:09:09 by preltien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

static void	pipex_close_fds(int *prev_fd, int pipe_fd[2], int is_last)
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
	perror("execve");
	exit(1);
}

static void	pipex_fork_and_exec(t_command *cmd, t_shell *state,
								int prev_fd, int pipe_fd[2], int is_last)
{
	setup_input_output(prev_fd, pipe_fd, is_last);
	exec_command_or_builtin(cmd, state);
}



static int	create_pipe_if_needed(int pipe_fd[2], int is_last)
{
	if (!is_last && pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (1);
	}
	return (0);
}

static int	fork_and_handle_child(t_command *cmd, t_shell *state,
								int prev_fd, int pipe_fd[2], int is_last)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
		pipex_fork_and_exec(cmd, state, prev_fd, pipe_fd, is_last);
	return (pid);
}

static void	wait_for_child(pid_t pid, t_shell *state)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		state->exit_code = WEXITSTATUS(status);
	else
		state->exit_code = 1;
}

int	pipex_exec_loop(t_command *cmds, t_shell *state)
{
	int		pipe_fd[2];
	int		prev_fd;
	pid_t	pid;
	int		is_last;

	prev_fd = -1;
	while (cmds)
	{
		is_last = (cmds->next == NULL);
		if (create_pipe_if_needed(pipe_fd, is_last))
			return (1);
		pid = fork_and_handle_child(cmds, state, prev_fd, pipe_fd, is_last);
		if (pid == -1)
			return (1);
		pipex_close_fds(&prev_fd, pipe_fd, is_last);
		if (!is_last)
			prev_fd = pipe_fd[0];
		wait_for_child(pid, state);
		cmds = cmds->next;
	}
	return (state->exit_code);
}


int	has_pipe(t_command *cmds)
{
	t_command	*current = cmds;

	while (current)
	{
		if (current->next)
			return (1);
		current = current->next;
	}
	return (0);
}

