/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:39:02 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/09 17:39:52 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * Libère la mémoire d'un tableau de chaînes (char **) se terminant par NULL.
 * Libère d'abord chaque chaîne, puis le tableau de pointeurs lui-même.
 *
 * array : Le tableau de chaînes à libérer.
 */

#include "libft.h"

void	ft_free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}
