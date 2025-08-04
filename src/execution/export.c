/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: preltien <preltien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 15:32:52 by preltien          #+#    #+#             */
/*   Updated: 2025/07/20 15:40:37 by preltien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char	**environ;

static int	handle_export_arg(t_shell *state, char *arg)
{
	char	*eq;
	int		name_len;
	char	varname[256];

	eq = strchr(arg, '=');
	if (!eq)
	{
		fprintf(stderr, "minishell: export: `%s`: invalid format\n", arg);
		return (1);
	}
	name_len = (int)(eq - arg);
	if (name_len >= (int)sizeof(varname))
	{
		fprintf(stderr, "minishell: export: `%s`: name too long\n", arg);
		return (1);
	}
	strncpy(varname, arg, name_len);
	varname[name_len] = '\0';
	if (!is_valid_varname(varname))
	{
		fprintf(stderr, "minishell: export: `%s`: not a valid id\n", varname);
		return (1);
	}
	if (set_env_var(state, varname, eq + 1) != 0)
	{
		fprintf(stderr, "minishell: export: failed to update env\n");
		return (1);
	}
	return (0);
}

int	builtin_export(t_shell *state, char **argv)
{
	int	i;
	int	ret;

	if (!argv[1])
	{
		print_env(state);
		return (0);
	}
	i = 1;
	ret = 0;
	while (argv[i])
	{
		if (handle_export_arg(state, argv[i]) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}

