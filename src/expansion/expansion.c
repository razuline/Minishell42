/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: preltien <preltien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 16:25:54 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/06 14:07:22 by preltien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_str(const char *value, t_shell *state);
static int	process_char(const char *value, int *i_ptr, char **res_ptr,
				t_shell *state);

int expand_token(t_token *head, t_shell *state)
{
    t_token *curr;
    char *orig_value;
    char *expanded_value;

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
		if (process_char(value, &i, &result, state) != 0)
		{
			free(result);
			return (NULL);
		}
	}
	return (result);
}

static int	process_char(const char *value, int *i_ptr, char **res_ptr,
		t_shell *state)
{
	if (value[*i_ptr] == '$' && value[*i_ptr + 1] != '\0')
	{
		(*i_ptr)++;
		if (value[*i_ptr] == '?')
		{
			(*i_ptr)++;
			return (append_exit_status(res_ptr, state));
		}
		else if (ft_isalnum(value[*i_ptr]) || value[*i_ptr] == '_')
		{
			return (append_env_var(res_ptr, value, i_ptr, state));
		}
		else
			return (append_dollar(res_ptr));
	}
	else
	{
		append_char(res_ptr, value[*i_ptr]);
		(*i_ptr)++;
	}
	return (0);
}
