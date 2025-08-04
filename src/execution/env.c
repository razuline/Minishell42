/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: preltien <preltien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 15:39:55 by preltien          #+#    #+#             */
/*   Updated: 2025/07/20 15:42:12 by preltien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char	**environ;

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

int	set_env_var(t_shell *state, const char *key, const char *value)
{
	int		i;
	size_t	key_len;
	char	*entry;
	char	**new_env;

	key_len = strlen(key);
	entry = NULL;
	entry = malloc(strlen(key) + strlen(value) + 2);
	if (!entry)
		return (1);
	sprintf(entry, "%s=%s", key, value);
	for (i = 0; state->envp[i]; i++)
	{
		if (strncmp(state->envp[i], key, key_len) == 0
			&& state->envp[i][key_len] == '=')
		{
			free(state->envp[i]);
			state->envp[i] = strdup(entry);
			return (0);
		}
	}
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
	{
		free(entry);
		return (1);
	}
	for (int j = 0; j < i; j++)
	{
		new_env[j] = strdup(state->envp[j]);
		if (!new_env[j])
		{
			while (j-- > 0)
				free(new_env[j]);
			free(new_env);
			free(entry);
			return (1);
		}
	}
	new_env[i] = entry;
	new_env[i + 1] = NULL;
	
	ft_free_array(state->envp);
	state->envp = NULL;
	state->envp = new_env;
	return (0);
}


char	**duplicate_environ(void)
{
	int		i;
	int		count;
	char	**copy;

	count = 0;
	while (environ[count])
		count++;
	copy = (char **)malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < count)
	{
		copy[i] = strdup(environ[i]);
		if (!copy[i])
		{
			while (--i >= 0)
				free(copy[i]);
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}