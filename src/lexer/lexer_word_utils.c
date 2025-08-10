/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:50:08 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/10 19:28:35 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_delimiter(char c)
{
	return (ft_isspace(c) || c == '|' || c == '<' || c == '>');
}

/* Bascule l'état des guillemets (dans/hors de ' ou "). */
int	update_quote_state(char *word, int type, int i)
{
	if (word[i] == '\'' && type != DOUBLE_QUOTE)
	{
		if (type == DEFAULT)
			type = SINGLE_QUOTE;
		else
			type = DEFAULT;
	}
	else if (word[i] == '"' && type != SINGLE_QUOTE)
	{
		if (type == DEFAULT)
			type = DOUBLE_QUOTE;
		else
			type = DEFAULT;
	}
	return (type);
}

/* Vérifie si un mot est entièrement entre guillemets. */
int	get_quote_type(char *word)
{
	int	len;

	len = ft_strlen(word);
	if (len >= 2 && word[0] == '\'' && word[len - 1] == '\'')
		return (SINGLE_QUOTE);
	else if (len >= 2 && word[0] == '"' && word[len - 1] == '"')
		return (DOUBLE_QUOTE);
	return (DEFAULT);
}

/* Trouve la fin d'un mot, en ignorant les délimiteurs entre guillemets. */
char	*find_word_end(char *word)
{
	int	i;
	int	type;

	if (!word)
		return (NULL);
	i = 0;
	type = DEFAULT;
	while (word[i])
	{
		if (type == DEFAULT && ft_delimiter(word[i]))
			break ;
		type = update_quote_state(word, type, i);
		i++;
	}
	return (word + i);
}

/* Nettoie une chaîne de ses guillemets (ex: "salut" -> salut). */
char	*remove_quotes_from_word(char *word)
{
	int		i;
	int		j;
	int		quote_state;
	char	*dest;

	if (!word)
		return (NULL);
	dest = malloc(ft_strlen(word) + 1);
	if (!dest)
		return (NULL);
	i = 0;
	j = 0;
	quote_state = DEFAULT;
	while (word[i])
	{
		if (word[i] == '\'' && quote_state != DOUBLE_QUOTE)
			quote_state = (quote_state == DEFAULT) * SINGLE_QUOTE;
		else if (word[i] == '"' && quote_state != SINGLE_QUOTE)
			quote_state = (quote_state == DEFAULT) * DOUBLE_QUOTE;
		else
			dest[j++] = word[i];
		i++;
	}
	dest[j] = '\0';
	return (dest);
}
