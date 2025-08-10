/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:58:00 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/10 16:06:43 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	print_export_error(char *arg);
static int	handle_export_with_value(t_shell *state, char *arg);
static int	handle_export_without_value(t_shell *state, char *arg);

/* Exporte une ou plusieurs variables dans l'environnement. */
int	builtin_export(char **argv, t_shell *state)
{
	int	i;
	int	ret_code;

	if (!argv[1])
	{
		print_env_sorted(state);
		return (0);
	}
	i = 1;
	ret_code = 0;
	while (argv[i])
	{
		if (ft_strchr(argv[i], '='))
		{
			if (handle_export_with_value(state, argv[i]) != 0)
				ret_code = 1;
		}
		else
		{
			if (handle_export_without_value(state, argv[i]) != 0)
				ret_code = 1;
		}
		i++;
	}
	return (ret_code);
}

/* Affiche un message d'erreur standardisé pour export. */
static int	print_export_error(char *arg)
{
	fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", arg);
	return (1);
}

/* Gère le cas où une valeur est assignée (ex: "VAR=valeur"). */
static int	handle_export_with_value(t_shell *state, char *arg)
{
	char	*key;
	char	*value;
	int		ret;

	value = ft_strchr(arg, '=');
	*value = '\0';
	key = arg;
	value++;
	if (!is_valid_varname(key))
	{
		*(--value) = '=';
		return (print_export_error(key));
	}
	ret = set_env_var(state, key, value);
	*(--value) = '=';
	return (ret);
}

/* Gère le cas où aucune valeur n'est assignée (ex: "VAR"). */
static int	handle_export_without_value(t_shell *state, char *arg)
{
	if (!is_valid_varname(arg))
		return (print_export_error(arg));
	if (!find_env_var(state->envp, arg))
		return (set_env_var(state, arg, NULL));
	return (0);
}
