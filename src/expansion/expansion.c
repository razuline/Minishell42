/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 16:25:54 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/05 17:30:10 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_and_fill(const char *src, char *dest, int *i, size_t *j,
				t_shell *state);
static char	*expand_str(const char *value, t_shell *state);
// static int	process_char(const char *value, int *i_ptr, char **res_ptr,
// 				t_shell *state);

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
			*j = append_str_to_res(dest, var_val, *j);
			free(var_val);
		}
		else if (ft_isalnum(src[*i]) || src[*i] == '_')
		{
			var_name_tmp = get_var_name(src, i);
			var_val = getenv(var_name_tmp);
			*j = append_str_to_res(dest, var_val, *j);
			free(var_name_tmp);
		}
		else
			dest[(*j)++] = '$';
	}
	else
		dest[(*j)++] = src[(*i)++];
}

static char	*expand_str(const char *value, t_shell *state)
{
	char	*result;
	size_t	final_len;
	int		i;
	size_t	j;

	if (!value)
		return (NULL);
	final_len = calcul_expanded_len(value, *state);
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
