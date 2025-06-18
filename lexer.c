/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 13:14:07 by erazumov          #+#    #+#             */
/*   Updated: 2025/06/18 15:09:11 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(t_token **head, t_token **tail, char *str, int type)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = ft_strdup(str);
	if (!new_token->value)
	{
		free(new_token);
		return (NULL);
	}
	new_token->type = type;
	new_token->next = NULL;
	if (*head == NULL)
	{
		*head = new_token;
		*tail = new_token;
	}
	else
	{
		(*tail)->next = new_token;
		*tail = new_token;
	}
	return (new_token);
}

void	lexer(char *line)
{
	char	*curr_char;
	t_token	*head;
	t_token	*tail;

	head = NULL;
	tail = NULL;
	curr_char = line;
	while (*curr_char)
	{
		while (ft_isspace(*curr_char))
			curr_char++;
		if (!*curr_char)
			break ;
		if (*curr_char == '|')
		{
			if (*(curr_char + 1) == '|')
			{
				create_token(&head, &tail, "||", OR_OPERATOR);
				curr_char += 2;
			}
			else
			{
				create_token(&head, &tail, "|", PIPE);
				curr_char++;
			}
		}
		else if (*curr_char == '>')
		{
			if (*(curr_char + 1) == '>')
			{
				create_token(&head, &tail, ">>", APPEND_OUT);
				curr_char += 2;
			}
			else
			{
				create_token(&head, &tail, ">", REDIRECT_OUT);
				curr_char++;
			}
		}
		else if (*curr_char == '<')
		{
			if (*(curr_char + 1) == '<')
			{
				create_token(&head, &tail, "<<", HEREDOC);
				curr_char += 2;
			}
			else
			{
				create_token(&head, &tail, "<", REDIRECT_IN);
				curr_char++;
			}
		}
		else if (*curr_char == ';')
		{
			create_token(&head, &tail, ";", SEMICOLON);
			curr_char++;
		}
		else if (*curr_char == '(')
		{
			create_token(&head, &tail, "(", PARENTHESIS_OPEN);
			curr_char++;
		}
		else if (*curr_char == ')')
		{
			create_token(&head, &tail, ")", PARENTHESIS_CLOSE);
			curr_char++;
		}
		else
		{

		}
	}
	print_token(head);
	free_token(head);
}

