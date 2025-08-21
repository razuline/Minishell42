/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:50:08 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/21 19:14:35 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Checks if a character is a word delimiter. */
static int	ft_delimiter(char c)
{
	return (ft_isspace(c) || c == '|' || c == '<' || c == '>');
}

/* Toggles the quote state (in/out of ' or "). */
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

/* Checks if a word is entirely enclosed in quotes. */
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

/* Finds the end of a word, ignoring delimiters inside quotes. */
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

/* Allocates and returns a new string by copying the word while
 * removing the outer quotes in a single pass (e.g., "hello" -> hello). */
char	*extract_and_clean_word(const char *start, int len)
{
	char	*dest;
	int		i;
	int		j;
	int		quote_state;

	dest = malloc(sizeof(char) * (len + 1));
	if (!dest)
		return (NULL);
	i = 0;
	j = 0;
	quote_state = DEFAULT;
	while (i < len)
	{
		if (start[i] == '\'' && quote_state != DOUBLE_QUOTE)
			quote_state = (quote_state == DEFAULT) * SINGLE_QUOTE;
		else if (start[i] == '"' && quote_state != SINGLE_QUOTE)
			quote_state = (quote_state == DEFAULT) * DOUBLE_QUOTE;
		else
			dest[j++] = start[i];
		i++;
	}
	dest[j] = '\0';
	return (dest);
}
