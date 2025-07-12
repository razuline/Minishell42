/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: preltien <preltien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 10:29:38 by erazumov          #+#    #+#             */
/*   Updated: 2025/07/11 18:26:57 by preltien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
static int	exec_cmd(char **cmd, t_shell *state)
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
		execve(cmd[0], cmd, state->envp);
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
int	execute(t_command *cmds, t_shell *state)
{
	t_command	*cmd;
	int			ret;

	cmd = cmds;
	ret = 0;
	if (!cmd)
	{
		fprintf(stderr, "[ERROR] cmd is NULL in execute\n");
		return (1);
	}
	while (cmd)
	{
		if (!cmd->argv || !cmd->argv[0])
		{
			fprintf(stderr, "[WARNING] Empty command, skipping...\n");
			cmd = cmd->next;
			continue ;
		}
		printf("is_builtin ?? %i\n", is_builtin(cmd->argv[0]));
		if (is_builtin(cmd->argv[0]))
			ret = exec_builtin(cmd->argv, state);
		else
		{
			get_absolute_path(cmd->argv, state);
			if (cmd->argv[0])
				ret = exec_cmd(cmd->argv, state);
			else
			{
				fprintf(stderr, "minishell: command not found: %s\n",
					cmd->argv[0]);
				ret = 127;
				state->exit_code = ret;
			}
		}
		cmd = cmd->next;
	}
	return (ret);
}
