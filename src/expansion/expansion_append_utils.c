/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_append_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 11:20:05 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/04 15:32:38 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	append_char(char **res_ptr, char c)
{
	char	buffer[2];
	char	*tmp;

	buffer[0] = c;
	buffer[1] = '\0';
	tmp = *res_ptr;
	*res_ptr = ft_strjoin(tmp, buffer);
	free(tmp);
	if (!*res_ptr)
		return (1);
	return (0);
}

int	append_dollar(char **res_ptr)
{
	return (append_char(res_ptr, '$'));
}

int	append_exit_status(char **res_ptr, t_shell *state)
{
	char	*exit_code_str;
	char	*tmp;

	exit_code_str = ft_itoa(state->exit_code);
	if (!exit_code_str)
		return (1);
	tmp = *res_ptr;
	*res_ptr = ft_strjoin(tmp, exit_code_str);
	free(tmp);
	free(exit_code_str);
	if (!*res_ptr)
		return (1);
	return (0);
}

size_t	append_str_to_res(char *dest, const char *src, size_t j)
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
