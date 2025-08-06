/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: preltien <preltien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 15:39:55 by preltien          #+#    #+#             */
/*   Updated: 2025/08/06 14:06:03 by preltien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char	**environ;

int	envp_len(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		i++;
	return (i);
}

char	*get_env_value(const char *name, char **envp)
{
	int	i;
	int	len;

	len = strlen(name);
	i = 0;
	while (envp[i])
	{
		if (strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (strdup(strchr(envp[i], '=') + 1));
		i++;
	}
	return (strdup(""));
}

// void	substitute_args(char **argv, char **envp)
// {
// 	int		i;
// 	char	*value;

// 	i = 0;
// 	while (argv[i])
// 	{
// 		if (argv[i][0] == '$' && strlen(argv[i]) > 1)
// 		{
// 			value = get_env_value(argv[i] + 1, envp);
// 			free(argv[i]);
// 			argv[i] = value;
// 		}
// 		i++;
// 	}
// }

void	print_env(t_shell *state)
{
	int		i;
	char	*eq;

	i = 0;
	while (state->envp && state->envp[i])
	{
		eq = strchr(state->envp[i], '=');
		if (eq)
			printf("export %.*s=\"%s\"\n", (int)(eq - state->envp[i]),
				state->envp[i], eq + 1);
		else
			printf("export %s\n", state->envp[i]);
		i++;
	}
}
