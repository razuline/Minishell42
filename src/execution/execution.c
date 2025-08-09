/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:33:07 by preltien          #+#    #+#             */
/*   Updated: 2025/08/09 14:45:47 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute_single_command(t_command *cmd, t_shell *state);
static int	execute_external_command(t_command *cmd, t_shell *state);
static void	run_child_process(t_command *cmd, t_shell *state);

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
		return (execute_builtin(cmd, state));
	else
		return (execute_external_command(cmd, state));
}

/* Aiguille vers la bonne fonction built-in et retourne son code de sortie. */
int	execute_builtin(char **argv, t_shell *state)
{
	if (ft_strcmp(argv[0], "echo") == 0)
		return (builtin_echo(argv));
	if (ft_strcmp(argv[0], "cd") == 0)
		return (builtin_cd(argv, state));
	if (ft_strcmp(argv[0], "pwd") == 0)
		return (builtin_pwd());
	if (ft_strcmp(argv[0], "env") == 0)
		return (builtin_env(state));
	if (ft_strcmp(argv[0], "export") == 0)
		return (builtin_export(argv, state));
	if (ft_strcmp(argv[0], "unset") == 0)
		return (builtin_unset(argv, state));
	if (ft_strcmp(argv[0], "exit") == 0)
		return (builtin_exit(argv, state));
	return (1);
}

/* Prépare et exécute une commande externe dans un nouveau processus. */
static int	execute_external_command(t_command *cmd, t_shell *state)
{
	get_absolute_path(cmd->argv, state);
	return (exec_cmd(cmd, state));
}

/* Logique exécutée dans le processus enfant (redirections,
		recherche de chemin, execve). */
static void	run_child_process(t_command *cmd, t_shell *state)
{
	if (apply_redirections(cmd->redir) < 0)
		exit(1);
	if (!cmd->argv || !cmd->argv[0])
		exit(0);
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
