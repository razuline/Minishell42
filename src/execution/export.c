/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 13:17:58 by preltien          #+#    #+#             */
/*   Updated: 2025/08/09 14:13:35 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	print_export_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
}

static int	handle_export_with_value(t_shell *state, char *arg, char *eq)
{
	char	varname[256];
	size_t	var_len;

	if (!eq)
		return (print_export_error(arg));
	var_len = eq - arg;
	if (var_len >= sizeof(varname))
		return (print_export_error(arg));
	ft_strlcpy(varname, arg, var_len + 1);
	if (!is_valid_varname(varname))
		return (print_export_error(arg));
	if (set_env_var(state, varname, eq + 1) != 0)
	{
		ft_putstr_fd("minishell: export: failed to update env\n", 2);
		return (1);
	}
	return (0);
}

static int	handle_export_without_value(t_shell *state, char *arg)
{
	if (!is_valid_varname(arg))
		return (print_export_error(arg));
	if (set_env_var(state, arg, NULL) != 0)
	{
		ft_putstr_fd("minishell: export: failed to update env\n", 2);
		return (1);
	}
	return (0);
}

static int	handle_export_arg(t_shell *state, char *arg)
{
	char	*eq;

	eq = ft_strchr(arg, '=');
	if (eq)
		return (handle_export_with_value(state, arg, eq));
	else
		return (handle_export_without_value(state, arg));
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
