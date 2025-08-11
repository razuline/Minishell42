/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:33:07 by preltien          #+#    #+#             */
/*   Updated: 2025/08/11 11:23:57 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute_single_command(t_command *cmd, t_shell *state);
static int	execute_external_command(t_command *cmd, t_shell *state);
static int	exec_cmd(t_command *cmd, t_shell *state);

/* Point d'entrée de l'exécution : aiguille vers une commande simple
		ou un pipeline. */
int	execute(t_command *cmds, t_shell *state)
{
	if (has_pipe(cmds))
		return (execute_pipeline(cmds, state));
	else
		return (execute_single_command(cmds, state));
}

/* Exécute une seule commande (qui n'est pas dans un pipeline). */
static int	execute_single_command(t_command *cmd, t_shell *state)
{
	if (!cmd->argv || !cmd->argv[0])
		return (0);
	if (is_builtin(cmd->argv[0]))
		return (execute_builtin(cmd->argv, state));
	else
		return (execute_external_command(cmd, state));
}

/* Prépare et exécute une commande externe dans un nouveau processus. */
static int	execute_external_command(t_command *cmd, t_shell *state)
{
	get_absolute_path(cmd->argv, state);
	return (exec_cmd(cmd, state));
}

/* Logique exécutée dans le processus enfant (redirections,
		recherche de chemin, execve). */
void	run_child_process(t_command *cmd, t_shell *state)
{
	if (apply_redirections(cmd->redir) < 0)
		exit(1);
	if (!cmd->argv || !cmd->argv[0])
		exit(0);
	if (is_builtin(cmd->argv[0]))
		exit(execute_builtin(cmd->argv, state));
	get_absolute_path(cmd->argv, state);
	if (is_directory(cmd->argv[0]))
	{
		fprintf(stderr, "minishell: %s: Is a directory\n", cmd->argv[0]);
		exit(126);
	}
	execve(cmd->argv[0], cmd->argv, state->envp);
	fprintf(stderr, "minishell: %s: %s\n", cmd->argv[0], strerror(errno));
	if (errno == ENOENT)
		exit(127);
	if (errno == EACCES)
		exit(126);
	exit(1);
}

/* Gère le fork et l'attente pour une seule commande externe. */
static int	exec_cmd(t_command *cmd, t_shell *state)
{
	pid_t	pid;
	int		status;

	setup_non_interactive_signals();
	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		return (1);
	}
	if (pid == 0)
	{
		setup_child_signals();
		run_child_process(cmd, state);
	}
	waitpid(pid, &status, 0);
	setup_interactive_signals();
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}
