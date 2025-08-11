/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 19:43:24 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/11 12:24:44 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Extracts, cleans, and creates a WORD token */
static int	word_token(t_token_lst *lst, char *start, char *end)
{
	int		len;
	char	*extracted;
	char	*cleaned;
	int		quote_type;

	len = end - start;
	extracted = ft_substr(start, 0, len);
	if (!extracted)
		return (1);
	quote_type = get_quote_type(extracted);
	cleaned = remove_quotes_from_word(extracted);
	if (!cleaned)
	{
		free(extracted);
		return (1);
	}
	if (create_token(lst, cleaned, WORD, quote_type) == NULL)
	{
		free(extracted);
		free(cleaned);
		return (1);
	}
	free(extracted);
	free(cleaned);
	return (0);
}

/* Isolates a word and starts its tokenisation */
int	handle_word(t_token_lst *lst, char **c)
{
	int		result;
	char	*wd_start;
	char	*wd_end;

	wd_start = *c;
	wd_end = find_word_end(wd_start);
	if (wd_start == wd_end)
	{
		*c = wd_end;
		return (0);
	}
	result = word_token(lst, wd_start, wd_end);
	*c = wd_end;
	return (result);
}

/* Creates a token for single operators ('|', '<', '>') */
int	handle_single_op(t_token_lst *lst, char **c)
{
	if (**c == '|')
	{
		create_token(lst, "|", PIPE, DEFAULT);
		*c += 1;
		return (1);
	}
	else if (**c == '<')
	{
		create_token(lst, "<", REDIRECT_IN, DEFAULT);
		*c += 1;
		return (1);
	}
	else if (**c == '>')
	{
		create_token(lst, ">", REDIRECT_OUT, DEFAULT);
		*c += 1;
		return (1);
	}
	return (0);
}

/* Creates a token for double operators ('<<', '>>') */
int	handle_double_op(t_token_lst *lst, char **c)
{
	if (**c == '<' && *(*c + 1) == '<')
	{
		create_token(lst, "<<", HEREDOC, DEFAULT);
		*c += 2;
		return (1);
	}
	else if (**c == '>' && *(*c + 1) == '>')
	{
		create_token(lst, ">>", APPEND_OUT, DEFAULT);
		*c += 2;
		return (1);
	}
	return (0);
}
