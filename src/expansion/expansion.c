/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 16:25:54 by erazumov          #+#    #+#             */
/*   Updated: 2025/06/27 13:04:28 by erazumov         ###   ########.fr       */
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
	if (!result)
		return (NULL);
	i = 0;
	while (value[i])
	{
		if (value[i] == '$')
			append_env_var(&result, value, &i);
		else if (value[i] == '$' && value[i + 1] == '?')
		{
			append_exit_status(&result, state);
			i += 2;
		}
		else
		{
			append_char(&result, value[i]);
			i++;
		}
	}
	return (result);
}
