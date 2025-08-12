/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 16:25:54 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/12 16:42:16 by erazumov         ###   ########.fr       */
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

/* Gère l'expansion de la variable $? (code de sortie). */
static void	expand_exit_status(char *dest, size_t *j, t_shell *state)
{
	char	*exit_code_str;

	exit_code_str = ft_itoa(state->exit_code);
	*j = append_str_to_result(dest, exit_code_str, *j);
	free(exit_code_str);
}

/* Gère l'expansion d'une variable d'environnement normale (ex: $USER). */
static void	expand_regular_var(const char *src, char *dest, int *i, size_t *j,
		t_shell *state)
{
	char	*var_name;
	char	*var_value;
	
	var_name = get_var_name(src, i);
	var_value = get_env_value(var_name, state->envp);
	*j = append_str_to_result(dest, var_value, *j);
	free(var_name);
	free(var_value);
}

/* Analyse un segment (caractère, $VAR, $?) et l'ajoute au résultat
 * en appelant le bon helper. */
static void	process_and_fill(const char *src, char *dest, int *i, size_t *j,
		t_shell *state)
{
	if (src[*i] == '$' && src[*i + 1])
	{
		(*i)++;
		if (src[*i] == '?')
		{
			(*i)++;
			expand_exit_status(dest, j, state);
		}
		else if (ft_isalnum(src[*i]) || src[*i] == '_')
			expand_regular_var(src, dest, i, j, state);
		else
			dest[(*j)++] = '$';
	}
	else
		dest[(*j)++] = src[(*i)++];
}

/* Orchestre l'expansion d'une chaîne en deux passes (calcul remplissage). */
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
