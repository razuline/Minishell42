/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:57:44 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/09 17:19:46 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Affiche les variables d'environnement qui ont une valeur. */
int	builtin_env(t_shell *state)
{
	int	i;

	i = 0;
	if (!state->envp)
		return (1);
	while (state->envp[i])
	{
		if (ft_strchr(state->envp[i], '='))
			printf("%s\n", state->envp[i]);
		i++;
	}
	return (0);
}
