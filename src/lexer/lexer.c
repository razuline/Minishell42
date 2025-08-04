/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 13:14:07 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/04 16:18:08 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	lexer_loop(t_token_lst *lst, char *line);

t_token	*lexer(char *line)
{
	t_token_lst	lst;

	if (!line)
		return (NULL);
	lst.head = NULL;
	lst.tail = NULL;
	lexer_loop(&lst, line);
	return (lst.head);
}

static void	lexer_loop(t_token_lst *lst, char *line)
{
	char	*c;

	c = line;
	while (*c)
	{
		while (*c && ft_isspace(*c))
			c++;
		if (!*c)
			break ;
		if (ft_double_token(lst, &c) == 1)
			;
		else if (ft_single_token(lst, &c) == 1)
			;
		else
			handle_word(lst, &c);
	}
	return (0);
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
		return (0);
	}
	free(extracted);
	free(cleaned);
	return (0);
}

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
