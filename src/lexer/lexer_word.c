/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 13:46:25 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/05 17:29:51 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_word_token(t_token_lst *lst, char *start, char *end);

int	handle_word(t_token_lst *lst, char **c)
{
	int		result;
	char	*wd_start;
	char	*wd_end;

	wd_start = *c;
	wd_end = ft_word_end(wd_start);
	if (wd_start == wd_end)
	{
		*c = wd_end;
		return (0);
	}
	result = ft_word_token(lst, wd_start, wd_end);
	*c = wd_end;
	return (result);
}

static int	ft_word_token(t_token_lst *lst, char *start, char *end)
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
	cleaned = delete_word_quotes(extracted);
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
