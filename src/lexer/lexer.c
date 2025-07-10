/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 13:14:07 by erazumov          #+#    #+#             */
/*   Updated: 2025/07/10 13:56:57 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*lexer(char *line)
{
	t_token	*head;
	t_token	*tail;
	t_list	lst;

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

static void	lexer_loop(t_list *lst, char *line)
{
	char	*cursor;

	cursor = line;
	while (*cursor)
	{
		while (*cursor && ft_isspace(*cursor))
			cursor++;
		if (!*cursor)
			break ;
		if (ft_double_token(lst, &cursor, &line) == 1)
			;
		else if (ft_single_token(lst, &cursor, &line) == 1)
			;
		else
			handle_word(lst, &cursor, &line);
	}
}
