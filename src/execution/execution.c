/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: preltien <preltien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 10:29:38 by erazumov          #+#    #+#             */
/*   Updated: 2025/07/11 11:26:16 by preltien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int	execute(t_command *cmd_lst, t_shell *state)
{
	pid_t	pid;
	int		status;
	char	*cmd_path;

	if (!cmd_lst || !cmd_lst->argv || !cmd_lst->argv[0])
		return (1);

	cmd_path = find_cmd_path(cmd_lst->argv[0], state->envp);
	if (!cmd_path)
		return (127); // Command not found

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		free(cmd_path);
		return (1);
	}
	if (pid == 0)
	{
		// Enfant : on remplace le processus par la commande
		execve(cmd_path, cmd_lst->argv, state->envp);
		perror("execve");
		free(cmd_path);
		exit(1);
	}
	// Parent : on attend la fin du fils
	waitpid(pid, &status, 0);
	free(cmd_path);

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (status);
}
