/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:50:08 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/04 15:19:54 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_delimiter(char c)
{
	return (ft_isspace(c) || c == '|' || c == '<' || c == '>');
}

int	upd_quote_state(char *word, int type, int i)
{
	if (word[i] == '\'' && type == DEFAULT)
		type = SINGLE_QUOTE;
	else if (word[i] == '"' && type == DEFAULT)
		type = DOUBLE_QUOTE;
	else if (word[i] == '\'' && type == SINGLE_QUOTE)
		type = DEFAULT;
	else if (word[i] == '"' && type == DOUBLE_QUOTE)
		type = DEFAULT;
	return (type);
}

/*
** Obtenir le type de guillemet.
*/
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

char	*ft_word_end(char *word)
{
	int		i;
	int		type;

	if (!word)
		return (NULL);
	i = 0;
	type = DEFAULT;
	while (word[i])
	{
		if (type == DEFAULT && ft_delimiter(word[i]))
			break ;
		type = upd_quote_state(word, type, i);
		i++;
	}
	return (word + i);
}
/*
** Prend une chaîne brute extraite par le lexer (ex: hello"'world')
** et retourne une nouvelle chaîne "propre" (helloworld).
*/
char	*ft_delete_word_quotes(char *word)
{
	int		i;
	int		j;
	int		quote_state;
	int		tmp_state;
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
		tmp_state = quote_state;
		quote_state = upd_quote_state(word, quote_state, i);
		if (quote_state == tmp_state)
			dest[j++] = word[i];
		i++;
	}
	dest[j] = '\0';
	return (dest);
}
