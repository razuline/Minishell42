/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 13:14:07 by erazumov          #+#    #+#             */
/*   Updated: 2025/07/12 15:51:01 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	lexer_loop(t_token_lst *lst, char *line);

t_token	*lexer(char *line)
{
	t_token		*head;
	t_token		*tail;
	t_token_lst	lst;

	if (!line)
		return (NULL);
	head = NULL;
	tail = NULL;
	lst.head = &head;
	lst.tail = &tail;
	if (!line)
		return (NULL);
	lexer_loop(&lst, line);
	return (head);
}

static void	lexer_loop(t_token_lst *lst, char *line)
{
	char	*cursor;

	cursor = line;
	while (*cursor)
	{
		while (*cursor && ft_isspace(*cursor))
			cursor++;
		if (!*cursor)
			break ;
		if (ft_double_token(lst, &cursor) == 1)
			;
		else if (ft_single_token(lst, &cursor) == 1)
			;
		else
			handle_word(lst, &cursor);
	}
}
