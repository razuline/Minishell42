/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:50:08 by erazumov          #+#    #+#             */
/*   Updated: 2025/06/20 17:42:28 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_delimiter(char c)
{
	return (ft_isspace(c) || c == '|' || c == '<' || c == '>');
}

int	ft_quote_type(char *word, int type, int i)
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
		type = ft_quote_type(word, type, i);
		i++;
	}
	return (word + i);
}

char	*ft_delete_quotes(char *word)
{
	char	*result;
	char	*start;
	char	*end;
	int		len;

	if (word == NULL)
		return (NULL);
	start = word;
	end = word;
	while (*end)
		end++;
	end--;
	len = ft_strlen(word);
	if (len >= 2 && (*start == '\'' || *start == '"') && *start == *end)
	{
		result = malloc(len - 2 + 1);
		if (!result)
			return (NULL);
		ft_memcpy(result, start + 1, len - 2);
		result[len - 2] = '\0';
		return (result);
	}
	else
		return (ft_strdup(word));
}
