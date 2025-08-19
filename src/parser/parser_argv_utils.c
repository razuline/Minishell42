/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_argv_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 13:04:53 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/19 18:40:59 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Copies the pointers from the old argv array to the new one. */
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

/* Counts the number of elements in a NULL-terminated string array (argv). */
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

/* Adds a new string to an argument vector (argv) by reallocating it.
 * This function creates a new, larger array, copies the old elements,
 * adds the new element, and frees the old array. */
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
	new_argv[count] = ft_strdup(new_str);
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
