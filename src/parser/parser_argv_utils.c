/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_argv_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 13:04:53 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/10 15:28:07 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Copie les pointeurs de l'ancien tableau vers le nouveau. */
static void	copy_old_argv(char **new_argv, char **old_argv, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		new_argv[i] = old_argv[i];
		i++;
	}
}

/* Compte le nombre d'éléments dans un tableau argv. */
static int	count_argv_elements(char **argv)
{
	int	count;

	count = 0;
	if (argv != NULL)
	{
		while (argv[count] != NULL)
			count++;
	}
	return (count);
}

/* Ajoute une nouvelle chaîne à un tableau d'arguments (argv),
	en le réallouant. */
char	**create_argv(char **old_argv, char *new_str)
{
	int		count;
	char	**new_argv;

	count = count_argv_elements(old_argv);
	new_argv = malloc(sizeof(char *) * (count + 2));
	if (!new_argv)
	{
		printf("[ERROR] create_argv: malloc failed\n");
		return (NULL);
	}
	copy_old_argv(new_argv, old_argv, count);
	new_argv[count] = strdup(new_str);
	if (!new_argv[count])
	{
		printf("[ERROR] create_argv: strdup failed for '%s'\n", new_str);
		free(new_argv);
		return (NULL);
	}
	new_argv[count + 1] = NULL;
	free(old_argv);
	return (new_argv);
}
