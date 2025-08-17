/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 16:25:54 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/17 17:39:35 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Gère l'expansion de la variable $? (code de sortie). */
static void	expand_exit_status(char *dest, size_t *j, t_shell *state)
{
	char	*exit_code_str;

	exit_code_str = ft_itoa(state->exit_code);
	*j = append_str_to_result(dest, exit_code_str, *j);
	free(exit_code_str);
}

/* Gère l'expansion d'une variable d'environnement normale (ex: $USER). */
static void	expand_regular_var(const char *src, char *dest, t_indices *indices,
		t_shell *state)
{
	char	*var_name;
	char	*var_value;

	var_name = get_var_name(src, &indices->i);
	var_value = get_env_value(var_name, state->envp);
	indices->j = append_str_to_result(dest, var_value, indices->j);
	free(var_name);
	free(var_value);
}

/* Analyse un segment (caractère, $VAR, $?) et l'ajoute au résultat
 * en appelant le bon helper. */
static void	process_and_fill(const char *src, char *dest, t_indices *indices,
		t_shell *state)
{
	if (src[indices->i] == '$' && src[indices->i + 1])
	{
		(indices->i)++;
		if (src[indices->i] == '?')
		{
			(indices->i)++;
			expand_exit_status(dest, &indices->j, state);
		}
		else if (ft_isalnum(src[indices->i]) || src[indices->i] == '_')
			expand_regular_var(src, dest, indices, state);
		else
			dest[indices->j++] = '$';
	}
	else
		dest[indices->j++] = src[indices->i++];
}

/* Orchestre l'expansion d'une chaîne en deux passes (calcul remplissage). */
static char	*expand_str(const char *value, t_shell *state)
{
	char		*result;
	size_t		final_len;
	t_indices	indices;

	if (!value)
		return (NULL);
	final_len = calculate_expanded_len(value, state);
	result = malloc(sizeof(char) * (final_len + 1));
	if (!result)
		return (NULL);
	indices.i = 0;
	indices.j = 0;
	while (value[indices.i])
		process_and_fill(value, result, &indices, state);
	result[indices.j] = '\0';
	return (result);
}

/* Parcours les tokens et expand les variables ($VAR, $?)
		pour ceux de type WORD. */
int	expand_token(t_token *head, t_shell *state)
{
	t_token	*curr;
	char	*orig_value;
	char	*expanded_value;

	if (!head)
		return (0);
	curr = head;
	while (curr != NULL)
	{
		if (curr->type == WORD && curr->quote_type != SINGLE_QUOTE)
		{
			orig_value = curr->value;
			expanded_value = expand_str(orig_value, state);
			if (!expanded_value)
				return (1);
			curr->value = expanded_value;
			free(orig_value);
		}
		curr = curr->next;
	}
	return (0);
}
