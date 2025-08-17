/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:58:14 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/17 17:42:15 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Termine le shell avec un code de sortie. */
int	builtin_exit(char **argv, t_shell *state)
{
	int	exit_code;

	ft_putstr_fd("exit\n", 2);
	if (!argv[1])
		exit(state->exit_code);
	if (!ft_str_is_numeric(argv[1]))
	{
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n",
			argv[1]);
		exit(255);
	}
	if (argv[2])
	{
		fprintf(stderr, "minishell: exit: too many arguments\n");
		return (1);
	}
	exit_code = ft_atoi(argv[1]);
	exit(exit_code % 256);
}
