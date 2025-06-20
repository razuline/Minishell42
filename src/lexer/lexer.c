/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 13:14:07 by erazumov          #+#    #+#             */
/*   Updated: 2025/06/20 14:42:18 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_token(t_token **head, t_token **tail, char **ch);
int	handle_word(t_token **head, t_token **tail, char **ch);

t_token	*lexer(char *line)
{
	char	*ch;
	char	*get_word;
	t_token	*head;
	t_token	*tail;

	ch = line;
	head = NULL;
	tail = NULL;
	if (!line)
		return (NULL);
	while (*ch)
	{
		while (*ch && ft_isspace(*ch))
			ch++;
		if (!*ch)
			break ;
		if (handle_token())

	}
	return (head);
}

int	handle_token(t_token **head, t_token **tail, char **ch)
{
	if (*ch == '|')
	{
		if (*(ch + 1) == '|')
		{
			create_token(&head, &tail, "||", OR_OPERATOR);
				ch += 2;
		}
		else
		{
			create_token(&head, &tail, "|", PIPE);
			ch++;
		}
	}
	else if (*ch == '>')
	{
		if (*(ch + 1) == '>')
		{
			create_token(&head, &tail, ">>", APPEND_OUT);
			ch += 2;
		}
		else
		{
			create_token(&head, &tail, ">", REDIRECT_OUT);
			ch++;
		}
	}
	else if (*ch == '<')
	{
		if (*(ch + 1) == '<')
		{
			create_token(&head, &tail, "<<", HEREDOC);
			ch += 2;
		}
		else
		{
			create_token(&head, &tail, "<", REDIRECT_IN);
			ch++;
		}
	}
	else if (*ch == ';')
	{
		create_token(&head, &tail, ";", SEMICOLON);
		ch++;
	}
	else if (*ch == '(')
	{
		create_token(&head, &tail, "(", PARENTHESIS_OPEN);
		ch++;
	}
	else if (*ch == ')')
	{
		create_token(&head, &tail, ")", PARENTHESIS_CLOSE);
		ch++;
	}
	else
		handle_word(&head, &tail, &ch);

}

int	handle_word(t_token **head, t_token **tail, char **ch)
{
	int		len;
	char	*start;
	char	*end;
	char	*extracted;
	char	*cleaned;

	start = ch;
	end = ft_word_end(start);
	len = end - start;
	if (len == 0)
	{
		*ch = end;
		return (0);
	}
	extracted = ft_substr(start, 0, len);
	if (!extracted)
		return (1);
	cleaned = ft_delete_quotes(extracted);
	if (!cleaned)
	{
		free(extracted);
		return (1);
	}
	create_token(head, tail, cleaned, WORD);
	free(extracted);
	free(cleaned);
	*ch = end;
	return (0);
}
