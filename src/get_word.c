/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_word.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:28:04 by erazumov          #+#    #+#             */
/*   Updated: 2025/06/19 14:16:00 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_word(char *word)
{
	char	*curr_char;
	char	*word_start;
	int		quote_type;
	int		word_len;
	char	*word_value;

	curr_char = word;
	word_start = curr_char;
	quote_type = DEFAULT;
	while (*curr_char)
	{
		if (quote_type == DEFAULT && ft_delimiter(*curr_char) == 1)
			break ;
		if (*curr_char == '\'' && quote_type == DEFAULT)
			quote_type = SINGLE_QUOTE;
		else if (*curr_char == '"' && quote_type == DEFAULT)
			quote_type = DOUBLE_QUOTE;
		else if (*curr_char == '\'' && quote_type == SINGLE_QUOTE)
			quote_type = DEFAULT;
		else if (*curr_char == '"' && quote_type == DOUBLE_QUOTE)
			quote_type = DEFAULT;
		curr_char++;
	}
	word_len = curr_char - word_start;
	word_value = ft_substr(word_start, 0, word_len);
}
