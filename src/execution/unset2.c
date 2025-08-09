/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 15:32:49 by preltien          #+#    #+#             */
/*   Updated: 2025/08/09 14:14:41 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	should_keep(const char *entry, const char *var)
{
	size_t	len;

	len = strlen(var);
	return (!(strncmp(entry, var, len) == 0 && entry[len] == '='));
}

static int	count_kept_vars(char **envp, const char *var)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (envp[i])
	{
		if (should_keep(envp[i], var))
			count++;
		i++;
	}
	return (count);
}

static char	**alloc_filtered_env(char **envp, const char *var)
{
	int		count;
	char	**new_env;

	count = count_kept_vars(envp, var);
	new_env = malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);
	return (new_env);
}

static void	copy_filtered_env(char **src, char **dest, const char *var)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (src[i])
	{
		if (!should_keep(src[i], var))
		{
			i++;
			continue ;
		}
		dest[j++] = strdup(src[i]);
		i++;
	}
	dest[j] = NULL;
}

int	remove_env_var(char ***envp, const char *var)
{
	char	**new_env;

	if (!envp || !*envp || !var)
		return (1);
	new_env = alloc_filtered_env(*envp, var);
	if (!new_env)
		return (1);
	copy_filtered_env(*envp, new_env, var);
	ft_free_array(*envp);
	*envp = new_env;
	return (0);
}
