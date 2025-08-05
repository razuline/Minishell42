/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: preltien <preltien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 15:39:55 by preltien          #+#    #+#             */
/*   Updated: 2025/08/04 16:46:41 by preltien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char	**environ;

static char	*create_env_entry(const char *key, const char *value)
{
	char	*entry;

	entry = malloc(strlen(key) + strlen(value) + 2);
	if (!entry)
		return (NULL);
	sprintf(entry, "%s=%s", key, value);
	return (entry);
}

static int	replace_env_var(char **envp, const char *key, const char *entry)
{
	int		i;
	size_t	key_len;

	i = 0;
	key_len = strlen(key);
	while (envp[i])
	{
		if (strncmp(envp[i], key, key_len) == 0 && envp[i][key_len] == '=')
		{
			free(envp[i]);
			envp[i] = strdup(entry);
			if (!envp[i])
				return (1);
			return (0);
		}
		i++;
	}
	return (-1);
}

int	envp_add_entry(char ***envp, char *entry)
{
	int		len;
	char	**new_env;

	len = envp_len(*envp);
	new_env = malloc(sizeof(char *) * (len + 2));
	if (!new_env)
		return (1);
	if (duplicate_env(*envp, new_env, len) != 0)
	{
		free(new_env);
		return (1);
	}
	new_env[len] = entry;
	new_env[len + 1] = NULL;
	ft_free_array(*envp);
	*envp = new_env;
	return (0);
}

int	set_env_var(t_shell *state, const char *key, const char *value)
{
	char	*entry;

	entry = create_env_entry(key, value);
	if (!entry)
		return (1);
	if (replace_env_var(state->envp, key, entry) == 0)
	{
		free(entry);
		return (0);
	}
	if (envp_add_entry(&state->envp, entry) != 0)
	{
		free(entry);
		return (1);
	}
	return (0);
}

int	duplicate_env(char **src, char **dst, int count)
{
	int	i;

	if (!src || !dst)
		return (1);
	i = 0;
	while (i < count)
	{
		dst[i] = strdup(src[i]);
		if (!dst[i])
		{
			while (--i >= 0)
				free(dst[i]);
			return (1);
		}
		i++;
	}
	return (0);
}
