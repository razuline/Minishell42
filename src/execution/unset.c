/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: preltien <preltien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 15:32:49 by preltien          #+#    #+#             */
/*   Updated: 2025/07/20 15:22:50 by preltien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;

static int	is_valid_varname(char *name)
{
	int	i;

	if (!name || !name[0])
		return (0);
	if (name[0] >= '0' && name[0] <= '9')
		return (0);
	i = 0;
	while (name[i])
	{
		if (!((name[i] >= 'a' && name[i] <= 'z') ||
			  (name[i] >= 'A' && name[i] <= 'Z') ||
			  (name[i] == '_') ||
			  (i > 0 && name[i] >= '0' && name[i] <= '9')))
			return (0);
		i++;
	}
	return (1);
}

static int	handle_unset_arg(t_shell *state, char *arg)
{
	if (!is_valid_varname(arg))
	{
		fprintf(stderr, "minishell: unset: `%s`: not a valid identifier\n", arg);
		return (1);
	}
	if (remove_env_var(&state->envp, arg) != 0)
	{
		perror("unset");
		return (1);
	}
	return (0);
}



int	builtin_unset(char **argv, t_shell *state)
{
	int	i;
	int	ret;

	if (!argv[1])
		return (0);
	i = 1;
	ret = 0;
	while (argv[i])
	{
		if (handle_unset_arg(state, argv[i]) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}


int	remove_env_var(char ***envp, const char *var)
{
	int		i, j, count;
	char	**new_env;

	if (!envp || !*envp || !var)
		return (1);

	for (count = 0, i = 0; (*envp)[i]; i++)
	{
		if (!(strncmp((*envp)[i], var, strlen(var)) == 0 && (*envp)[i][strlen(var)] == '='))
			count++;
	}

	new_env = malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (1);

	for (i = 0, j = 0; (*envp)[i]; i++)
	{
		if (strncmp((*envp)[i], var, strlen(var)) == 0 && (*envp)[i][strlen(var)] == '=')
		{
			free((*envp)[i]);
			continue;
		}
		new_env[j++] = strdup((*envp)[i]);
	}
	new_env[j] = NULL;

	ft_free_array(*envp);
	*envp = new_env;
	return (0);
}


