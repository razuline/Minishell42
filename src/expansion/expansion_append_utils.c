/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_append_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 11:20:05 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/06 20:42:55 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* (2ème passe) Ajoute une chaîne source à la chaîne de destination. */
size_t	append_str_to_result(char *dest, const char *src, size_t j)
{
	int	k;

	k = 0;
	while (src && src[k])
	{
		dest[j + k] = src[k];
		k++;
	}
	return (j + k);
}
