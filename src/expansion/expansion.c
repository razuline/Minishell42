/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 16:25:54 by erazumov          #+#    #+#             */
/*   Updated: 2025/06/27 12:04:22 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_str(const char *value, t_shell *state);

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

static char	*expand_str(const char *value, t_shell *state)
{
	char	*result;
	int		i;

	if (!value)
		return (NULL);
	result = ft_strdup("");
	i = 0;
	while (value[i])
	{
		if (value[i] == '$' && value[i + 1] != '\0'
			&& !ft_isspace(value[i + 1]))
		{
			i++;
			if (value[i] == '?')
			{
				append_exit_status(&result, state);
				i++;
			}
			else
				append_env_var(&result, value, &i);
		}
		append_char(&result, value[i]);
		i++;
	}
	return (result);
}
