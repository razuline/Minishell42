/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 15:39:55 by preltien          #+#    #+#             */
/*   Updated: 2025/08/09 14:11:35 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
