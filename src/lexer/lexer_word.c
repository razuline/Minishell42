/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: preltien <preltien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 13:46:25 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/06 16:19:08 by preltien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static int	ft_word_token(t_tokenlist *lst, char *start, char *end);

char	*strjoin_and_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

int	handle_word(t_tokenlist *lst, char **c)
{
	char	*start;
	char	*tmp;
	char	quote;

	char *final = ft_strdup(""); // initialise une string vide
	if (!final)
		return (1);
	while (**c && !ft_delimiter(**c))
	{
		if (**c == '\'' || **c == '"')
		{
			quote = **c;
			(*c)++; // skip opening quote
			start = *c;
			while (**c && **c != quote)
				(*c)++;
			tmp = ft_substr(start, 0, *c - start);
			if (!tmp)
				return (free(final), 1);
			final = strjoin_and_free(final, tmp);
			free(tmp);
			if (**c == quote)
				(*c)++; // skip closing quote
		}
		else
		{
			start = *c;
			while (**c && !ft_delimiter(**c) && **c != '\'' && **c != '"')
				(*c)++;
			tmp = ft_substr(start, 0, *c - start);
			if (!tmp)
				return (free(final), 1);
			final = strjoin_and_free(final, tmp);
			free(tmp);
		}
	}
	create_token(lst, final, WORD, DEFAULT);
	free(final);
	return (0);
}
