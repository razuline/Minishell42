/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: preltien <preltien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 10:39:58 by erazumov          #+#    #+#             */
/*   Updated: 2025/07/20 15:42:06 by preltien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <linux/limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern char	**environ;

/*
 * Vérifie si cmd est une builtin connue
 */
int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (strcmp(cmd, "echo") == 0 || strcmp(cmd, "cd") == 0 || strcmp(cmd,
			"pwd") == 0 || strcmp(cmd, "env") == 0 || strcmp(cmd, "export") == 0
		|| strcmp(cmd, "unset") == 0 || strcmp(cmd, "exit") == 0);
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
		state->exit_code = builtin_export(state, argv);
	else if (strcmp(argv[0], "unset") == 0)
		state->exit_code = builtin_unset(argv, state);
	else if (strcmp(argv[0], "exit") == 0)
		state->exit_code = builtin_exit(argv);
	else
		state->exit_code = 1;
	return (state->exit_code);
}

void	ft_free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

static int	is_valid_varname(char *name)
{
	int i;

	if (!name || !name[0])
		return (0);
	if (name[0] >= '0' && name[0] <= '9')
		return (0);
	i = 0;
	while (name[i])
	{
		if (!((name[i] >= 'a' && name[i] <= 'z') || (name[i] >= 'A'
					&& name[i] <= 'Z') || (name[i] == '_') || (i > 0
					&& name[i] >= '0' && name[i] <= '9')))
			return (0);
		i++;
	}
	return (1);
}