/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_var_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 11:19:48 by erazumov          #+#    #+#             */
/*   Updated: 2025/07/12 13:09:04 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_var_name(const char *input, int *i_ptr);
static int	is_valid_var(char c);

// Gère le remplacement d'une variable d'environnement ($VAR)
int	append_env_var(char **res_ptr, const char *input, int *i_ptr)
{
	char	*var_name;
	char	*value;
	char	*tmp;

	var_name = get_var_name(input, i_ptr);
	if (!var_name)
		return (append_dollar(res_ptr));
	value = getenv(var_name);
	free(var_name);
	if (value != NULL)
	{
		tmp = *res_ptr;
		*res_ptr = ft_strjoin(tmp, value);
		free(tmp);
		if (!*res_ptr)
			return (1);
	}
	return (0);
}

// Extrait un nom de variable valide depuis une chaîne et avance le curseur
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

// Vérifie si un caractère est valide pour un nom de variable
static int	is_valid_var(char c)
{
	return (ft_isalnum(c) || c == '_');
}
