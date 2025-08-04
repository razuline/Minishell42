#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

/*
 * Exécute une commande externe via fork + execve
 */
static int exec_cmd(t_command *cmd, t_shell *state)
{
	pid_t	pid;
	int		status;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		state->exit_code = 1;
		return (1);
	}
	else if (pid == 0)
	{
		if (cmd->redir)
		{
			t_redir *r = cmd->redir;
			while (r)
			{
				r = r->next;
			}
		}
		else

		if (apply_redirections(cmd->redir) < 0)
		{
			fprintf(stderr, "[ERROR] Failed to apply redirections\n");
			exit(EXIT_FAILURE);
		}
		execve(cmd->argv[0], cmd->argv, state->envp);
		perror("minishell");
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			state->exit_code = WEXITSTATUS(status);
		else
			state->exit_code = 1;
		return (state->exit_code);
	}
}

/*
 * Parcourt la liste des commandes et les exécute
 */
int execute(t_command *cmds, t_shell *state)
{
    t_command *cmd;
    int ret = 0;

    if (!cmds)
    {
        fprintf(stderr, "[ERROR] cmd is NULL in execute\n");
        return (1);
    }

    if (has_pipe(cmds))
        return pipex_exec_loop(cmds, state);

    cmd = cmds;

    if (!cmd->argv || !cmd->argv[0])
    {
        fprintf(stderr, "[WARNING] Empty command, skipping...\n");
        return (0);
    }

    if (is_builtin(cmd->argv[0]))
    {
        // ✅ Exécuter dans le parent pour les builtins
        if (apply_redirections(cmd->redir) < 0)
        {
            fprintf(stderr, "[ERROR] Failed to apply redirections\n");
            return (1);
        }
        ret = exec_builtin(cmd->argv, state);
        state->exit_code = ret;
        return ret;
    }
    else
    {
        get_absolute_path(cmd->argv, state);
        if (cmd->argv[0])
        {
            ret = exec_cmd(cmd, state);
        }
        else
        {
            fprintf(stderr, "minishell: command not found: %s\n",
                    cmd->argv[0]);
            ret = 127;
        }
    }

    state->exit_code = ret;
    return ret;
}


