/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_var_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 11:19:48 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/05 17:30:46 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var_name(const char *input, int *i_ptr);
static int	is_valid_var(char c);

static char	*get_var_name(const char *input, int *i_ptr)
{
	int		len;
	int		start_pos;
	char	*var_name;

	len = 0;
	start_pos = *i_ptr;
	while (input[start_pos + len] && is_valid_var(input[start_pos + len]))
		len++;
	if (len == 0)
		return (NULL);
	var_name = ft_substr(input, start_pos, len);
	*i_ptr += len;
	return (var_name);
}

static int	is_valid_var(char c)
{
	return (ft_isalnum(c) || c == '_');
}
