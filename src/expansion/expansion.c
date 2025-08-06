/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 16:25:54 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/06 20:43:37 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_and_fill(const char *src, char *dest, int *i, size_t *j,
				t_shell *state);
static char	*expand_str(const char *value, t_shell *state);

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

/* (2ème passe) Gère un segment (caractère, $VAR, $?)
		et l'ajoute au résultat. */
static void	process_and_fill(const char *src, char *dest, int *i, size_t *j,
		t_shell *state)
{
	char	*var_val;
	char	*var_name_tmp;

	if (src[*i] == '$' && src[*i + 1])
	{
		(*i)++;
		if (src[*i] == '?')
		{
			(*i)++;
			var_val = ft_itoa(state->exit_code);
			*j = append_str_to_result(dest, var_val, *j);
			free(var_val);
		}
		else if (ft_isalnum(src[*i]) || src[*i] == '_')
		{
			var_name_tmp = get_var_name(src, i);
			var_val = getenv(var_name_tmp);
			*j = append_str_to_result(dest, var_val, *j);
			free(var_name_tmp);
		}
		else
			dest[(*j)++] = '$';
	}
	else
		dest[(*j)++] = src[(*i)++];
}

/* Orchestre l'expansion d'une chaîne en deux passes (calcul puis remplissage). */
static char	*expand_str(const char *value, t_shell *state)
{
	char	*result;
	size_t	final_len;
	int		i;
	size_t	j;

	if (!value)
		return (NULL);
	final_len = calculate_expanded_len(value, state);
	result = malloc(sizeof(char) * (final_len + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (value[i])
		process_and_fill(value, result, &i, &j, state);
	result[j] = '\0';
	return (result);
}
