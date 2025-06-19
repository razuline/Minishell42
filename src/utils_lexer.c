/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:50:08 by erazumov          #+#    #+#             */
/*   Updated: 2025/06/19 16:03:15 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_delimiter(char c)
{
	return (c == ft_isspace(c) || c == '|' || c == '<' || c == '>');
}

char	*ft_delete_quotes(char *wd_value)
{
	char	*result;
	char	*start;
	char	*end;
	int		len;

	if (wd_value == NULL)
		return (NULL);
	start = wd_value;
	end = wd_value;
	while (*end)
		end++;
	end--;
	len = ft_strlen(wd_value);
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
		return (ft_strdup(wd_value));
}

char	*ft_word_end(char *word)
{
	char	*curr_ch;
	int		quote_type;

	if (word == NULL)
		return (NULL);
	curr_ch = word;
	quote_type = DEFAULT;
	while (*curr_ch)
	{
		if (quote_type == DEFAULT && ft_delimiter(*curr_ch))
			break ;
		else
		{
			if (quote_type == DEFAULT && *curr_ch == '\'')
				quote_type = SINGLE_QUOTE;
			else if (quote_type == DEFAULT && *curr_ch == '"')
				quote_type = DOUBLE_QUOTE;
			else if (quote_type == SINGLE_QUOTE && *curr_ch == '\'')
				quote_type = DEFAULT;
			else if (quote_type == DOUBLE_QUOTE && *curr_ch == '"')
				quote_type = DEFAULT; 
		}
		curr_ch++;
	}
	return (curr_ch);
}
