/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_var_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: preltien <preltien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 11:19:48 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/07 10:32:56 by preltien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_char(char c);

/* Extrait un nom de variable valide (ex: "USER" depuis "$USER_PATH"). */
char	*get_var_name(const char *input, int *i_ptr)
{
	int		len;
	int		start_pos;
	char	*var_name;

	len = 0;
	start_pos = *i_ptr;
	while (input[start_pos + len] && is_valid_char(input[start_pos + len]))
		len++;
	if (len == 0)
		return (NULL);
	var_name = ft_substr(input, start_pos, len);
	*i_ptr += len;
	return (var_name);
}

/* Vérifie si un caractère est valide dans un nom de variable
		(alphanumérique ou '_'). */
static int	is_valid_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}
