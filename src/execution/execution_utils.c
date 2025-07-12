/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: preltien <preltien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 10:39:58 by erazumov          #+#    #+#             */
/*   Updated: 2025/07/11 18:12:35 by preltien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <linux/limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

/*
 * Vérifie si cmd est une builtin connue
 */
int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (
		strcmp(cmd, "echo") == 0
		|| strcmp(cmd, "cd") == 0
		|| strcmp(cmd, "pwd") == 0
		|| strcmp(cmd, "env") == 0
		|| strcmp(cmd, "export") == 0
		|| strcmp(cmd, "unset") == 0
		|| strcmp(cmd, "exit") == 0
	);
}

/*
 * Exécute la commande builtin et met à jour l’état
 */
int	exec_builtin(char **argv, t_shell *state)
{
	if (!argv || !argv[0])
		return (1);

	if (strcmp(argv[0], "echo") == 0)
		state->exit_code = builtin_echo(argv);
	else if (strcmp(argv[0], "cd") == 0)
		state->exit_code = builtin_cd(argv);
	else if (strcmp(argv[0], "pwd") == 0)
		state->exit_code = builtin_pwd();
	else if (strcmp(argv[0], "env") == 0)
		state->exit_code = builtin_env(state->envp);
	else if (strcmp(argv[0], "export") == 0)
		state->exit_code = builtin_export(argv);
	else if (strcmp(argv[0], "unset") == 0)
		state->exit_code = builtin_unset(argv);
	else if (strcmp(argv[0], "exit") == 0)
		state->exit_code = builtin_exit(argv);
	else
		state->exit_code = 1;

	return (state->exit_code);
}


void	get_absolute_path(char **cmd, t_shell *state)
{
	char	*path_env = NULL;
	char	*bin = NULL;
	char	**path_split = NULL;

	for (int i = 0; state->envp[i]; i++)
	{
		if (strncmp(state->envp[i], "PATH=", 5) == 0)
		{
			path_env = strdup(state->envp[i] + 5);
			break;
		}
	}
	if (!path_env)
		path_env = strdup("/home/preltien/bin:/home/preltien/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin");

	if (cmd[0][0] != '/' && strncmp(cmd[0], "./", 2) != 0)
	{
		path_split = ft_split(path_env, ':');
		for (int i = 0; path_split[i]; i++)
		{
			size_t	len = strlen(path_split[i]) + strlen(cmd[0]) + 2;
			bin = (char *)calloc(len, sizeof(char));
			if (!bin)
				break;
			strcpy(bin, path_split[i]);
			strcat(bin, "/");
			strcat(bin, cmd[0]);
			if (access(bin, F_OK | X_OK) == 0)
				break;
			free(bin);
			bin = NULL;
		}
		ft_free_array(path_split);
		if (bin)
		{
			free(cmd[0]);
			cmd[0] = bin;
		}
	}
	free(path_env);
}


void	ft_free_array(char **array)
{
	if (!array)
		return;
	for (int i = 0; array[i]; i++)
		free(array[i]);
	free(array);
}
