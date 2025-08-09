/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:57:28 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/09 17:19:27 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Affiche les arguments passés à la commande, avec gestion de l'option -n. */
int	builtin_echo(char **argv)
{
	int	i;
	int	newline_flag;

	i = 1;
	newline_flag = 1;
	if (argv[i] && ft_strcmp(argv[i], "-n") == 0)
	{
		newline_flag = 0;
		i++;
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		if (argv[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (newline_flag)
		ft_putstr_fd("\n", 1);
	return (0);
}
