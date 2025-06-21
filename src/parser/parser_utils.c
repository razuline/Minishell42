/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 13:04:53 by erazumov          #+#    #+#             */
/*   Updated: 2025/06/21 13:33:26 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**create_argv(char **old_argv, char *new_str)
{
	int		i;
	int		count;
	char	**new_argv;

	count = 0;
	if (old_argv != NULL)
	{
		while (old_argv[count] != NULL)
		count++;
	}
	new_argv = malloc(sizeof(char *) * (count + 2));
	if (!new_argv)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_argv[i] = old_argv[i];
		i++;
	}
	new_argv[count] = new_str;
	new_argv[count + 1] = NULL;
	free(old_argv);
	return (new_argv);
}
