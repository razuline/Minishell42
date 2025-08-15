/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 14:11:22 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/15 14:11:47 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
 * @brief Appends the src string to the dest string, overwriting the
 * terminating null byte ('\0') at the end of dest, and then adds a
 * terminating null byte.
 *
 * @param dest The destination string, which should have enough space for the result.
 * @param src  The source string to be appended.
 * @return A pointer to the resulting string dest.
 */

#include "libft.h"

char	*ft_strcat(char *dest, const char *src)
{
	int	dest_len;
	int	i;

	dest_len = 0;
	// First, find the end of the destination string.
	while (dest[dest_len] != '\0')
	{
		dest_len++;
	}
	i = 0;
	// Now, copy the source string starting from the end of the destination.
	while (src[i] != '\0')
	{
		dest[dest_len + i] = src[i];
		i++;
	}
	// Add the null terminator to the very end of the newly formed string.
	dest[dest_len + i] = '\0';
	// Return the pointer to the destination.
	return (dest);
}