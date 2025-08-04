#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

extern char	**environ;

/*
 * Exécute une commande externe via fork + execve
 */
static void	child_process(t_command *cmd, t_shell *state)
{
	if (apply_redirections(cmd->redir) < 0)
	{
		fprintf(stderr, "[ERROR] Failed to apply redirections\n");
		exit(EXIT_FAILURE);
	}
	execve(cmd->argv[0], cmd->argv, state->envp);
	perror("minishell");
	exit(EXIT_FAILURE);
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

static int	exec_cmd(t_command *cmd, t_shell *state)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		state->exit_code = 1;
		return (1);
	}
	if (pid == 0)
		child_process(cmd, state);
	return (parent_process(pid, state));
}


/*
 * Parcourt la liste des commandes et les exécute
 */
static int	execute_builtin_cmd(t_command *cmd, t_shell *state)
{
	int	saved_stdin;
	int	saved_stdout;
	int	ret;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (apply_redirections(cmd->redir) < 0)
	{
		fprintf(stderr, "[ERROR] Failed to apply redirections\n");
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return (1);
	}
	ret = exec_builtin(cmd->argv, state);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	state->exit_code = ret;
	return (ret);
}

static int	execute_external_cmd(t_command *cmd, t_shell *state)
{
	int	ret;

	get_absolute_path(cmd->argv, state);
	if (cmd->argv[0])
		ret = exec_cmd(cmd, state);
	else
	{
		fprintf(stderr, "minishell: command not found: %s\n", cmd->argv[0]);
		ret = 127;
	}
	state->exit_code = ret;
	return (ret);
}

int	execute(t_command *cmds, t_shell *state)
{
	t_command	*cmd;
	int			ret;

	if (!cmds)
	{
		fprintf(stderr, "[ERROR] cmd is NULL in execute\n");
		return (1);
	}
	if (has_pipe(cmds))
		return (pipex_exec_loop(cmds, state));
	cmd = cmds;
	if (!cmd->argv || !cmd->argv[0])
	{
		fprintf(stderr, "[WARNING] Empty command, skipping...\n");
		return (0);
	}
	substitute_args(cmd->argv, state->envp);
	if (is_builtin(cmd->argv[0]))
		ret = execute_builtin_cmd(cmd, state);
	else
		ret = execute_external_cmd(cmd, state);
	return (ret);
}

