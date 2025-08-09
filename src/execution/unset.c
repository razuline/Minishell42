/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 15:32:49 by preltien          #+#    #+#             */
/*   Updated: 2025/08/09 14:14:36 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_unset_arg(t_shell *state, char *arg)
{
	if (!is_valid_varname(arg))
	{
		fprintf(stderr, "minishell: unset: `%s`: not a valid identifier\n",
			arg);
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
		if (process_unset_arg(state, argv[i]) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}
