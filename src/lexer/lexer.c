/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 13:14:07 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/04 15:15:33 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			ft_single_token(t_token **head, t_token **tail, char **c);
int			ft_double_token(t_token **head, t_token **tail, char **c);
int			handle_word(t_token **head, t_token **tail, char **ch);

t_token	*lexer(char *line)
{
	char	*c;
	t_token	*head;
	t_token	*tail;

	c = line;
	head = NULL;
	tail = NULL;
	if (!line)
		return (NULL);
	while (*c)
	{
		while (*c && ft_isspace(*c))
			c++;
		if (!*c)
			break ;
		if (ft_double_token(&head, &tail, &c) == 1)
			;
		else if (ft_single_token(&head, &tail, &c) == 1)
			;
		else
			handle_word(&head, &tail, &c);
	}
	return (head);
}

int	ft_single_token(t_token **head, t_token **tail, char **c)
{
	if (**c == '|')
	{
		create_token(head, tail, "|", PIPE, DEFAULT);
		*c += 1;
		return (1);
	}
	else if (**c == '<')
	{
		create_token(head, tail, "<", REDIRECT_IN, DEFAULT);
		*c += 1;
		return (1);
	}
	else if (**c == '>')
	{
		create_token(head, tail, ">", REDIRECT_OUT, DEFAULT);
		*c += 1;
		return (1);
	}
	return (0);
}

int	ft_double_token(t_token **head, t_token **tail, char **c)
{
	if (**c == '<' && *(*c + 1) == '<')
	{
		create_token(head, tail, "<<", HEREDOC, DEFAULT);
		*c += 2;
		return (1);
	}
	else if (**c == '>' && *(*c + 1) == '>')
	{
		create_token(head, tail, ">>", APPEND_OUT, DEFAULT);
		*c += 2;
		return (1);
	}
	return (0);
}

static int	ft_word_token(t_token **head, t_token **tail, char *start,
		char *end)
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
	cleaned = ft_delete_word_quotes(extracted);
	if (!cleaned)
	{
		free(extracted);
		return (1);
	}
	create_token(head, tail, cleaned, WORD, quote_type);
	free(extracted);
	free(cleaned);
	return (0);
}

int	handle_word(t_token **head, t_token **tail, char **c)
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
	result = ft_word_token(head, tail, wd_start, wd_end);
	*c = wd_end;
	return (result);
}
