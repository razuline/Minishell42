/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: preltien <preltien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 15:32:52 by preltien          #+#    #+#             */
/*   Updated: 2025/08/04 16:43:07 by preltien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char	**environ;

static int	print_export_error(const char *msg, const char *arg)
{
	fprintf(stderr, "minishell: export: `%s`: %s\n", arg, msg);
	return (1);
}

static int	extract_varname(char *arg, char *varname, int size)
{
	char	*eq;
	int		len;

	eq = strchr(arg, '=');
	if (!eq)
		return (print_export_error("invalid format", arg));
	len = eq - arg;
	if (len >= size)
		return (print_export_error("name too long", arg));
	strncpy(varname, arg, len);
	varname[len] = '\0';
	if (!is_valid_varname(varname))
		return (print_export_error("not a valid id", varname));
	return (0);
}

static int	set_env_var_from_arg(t_shell *state, char *arg)
{
	char	varname[256];
	char	*eq;

	eq = strchr(arg, '=');
	if (extract_varname(arg, varname, sizeof(varname)) != 0)
		return (1);
	if (set_env_var(state, varname, eq + 1) != 0)
	{
		fprintf(stderr, "minishell: export: failed to update env\n");
		return (1);
	}
	return (0);
}

static int	handle_export_arg(t_shell *state, char *arg)
{
	return (set_env_var_from_arg(state, arg));
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
