/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:58:48 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/18 10:27:55 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Built-in command: unset.
 * Deletes one or more variables from the environment. */
int	builtin_unset(char **argv, t_shell *state)
{
	int	i;
	int	ret_code;

	i = 1;
	ret_code = 0;
	while (argv[i])
	{
		if (!is_valid_varname(argv[i]))
		{
			fprintf(stderr, "minishell: unset: `%s': not a valid identifier\n",
				argv[i]);
			ret_code = 1;
		}
		else
			unset_env_var(state, argv[i]);
		i++;
	}
	return (ret_code);
}
