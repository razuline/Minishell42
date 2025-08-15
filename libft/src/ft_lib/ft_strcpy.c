/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 14:10:18 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/15 14:10:42 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
 * @brief Copies the string pointed to by src, including the terminating
 * null byte ('\0'), to the buffer pointed to by dest.
 *
 * @param dest Pointer to the destination array where the content is to be copied.
 * @param src  The string to be copied.
 * @return A pointer to the destination string dest.
 */

#include "libft.h"

char	*ft_strcpy(char *dest, const char *src)
{
	int	i;

	i = 0;
	// Loop through the source string until the null terminator is reached.
	while (src[i] != '\0')
	{
		// Copy each character from source to destination.
		dest[i] = src[i];
		i++;
	}
	// Add the null terminator to the end of the destination string.
	dest[i] = '\0';
	// Return the pointer to the destination.
	return (dest);
}
