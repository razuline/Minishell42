/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_len_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 15:21:15 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/05 17:23:26 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	get_len_var(const char *input, int *i_ptr);
static size_t	get_len_exit_status(t_shell *state);

/*
** Calcule la longueur finale de la chaîne après expansion.
*/
size_t	calcul_expanded_len(const char *value, t_shell *state)
{
	int		i;
	size_t	len;

	i = 0;
	len = 0;
	while (value[i])
	{
		if (value[i] == '$' && value[i + 1] != '\0')
		{
			i++;
			if (value[i] == '?')
			{
				i++;
				len += get_len_exit_status(state);
			}
			else if (ft_isalnum(value[i]) || value[i] == '_')
				len += get_len_var(value, &i);
			else
				len++;
		}
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

/*
** Calcule la longueur d'une variable d'environnement.
*/
static size_t	get_len_var(const char *input, int *i_ptr)
{
	char	*var_name;
	char	*value;
	size_t	len;
	int		start_pos;
	int		name_len;

	start_pos = *i_ptr;
	name_len = 0;
	while (input[start_pos + name_len] && (ft_isalnum(input[start_pos
				+ name_len]) || input[start_pos + name_len] == '_'))
		name_len++;
	if (name_len == 0)
		return (0);
	var_name = ft_substr(input, start_pos, name_len);
	*i_ptr += name_len;
	if (!var_name)
		return (0);
	value = getenv(var_name);
	free(var_name);
	if (!value)
		return (0);
	len = ft_strlen(value);
	return (len);
}

/*
** Calcule la longueur du code de sortie (converti en chaîne).
*/
static size_t	get_len_exit_status(t_shell *state)
{
	char	*exit_code_str;
	size_t	len;

	exit_code_str = ft_itoa(state->exit_code);
	if (!exit_code_str)
		return (0);
	len = ft_strlen(exit_code_str);
	free(exit_code_str);
	return (len);
}
