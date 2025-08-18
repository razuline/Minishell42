/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 14:19:24 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/18 11:02:27 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Allocates a new token and adds it to the end of the list. */
t_token	*create_token(t_token_lst *lst, char *word, int type, int quote_info)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = ft_strdup(word);
	if (!new_token->value)
	{
		free(new_token);
		return (NULL);
	}
	new_token->type = type;
	new_token->quote_type = quote_info;
	new_token->next = NULL;
	if (lst->head == NULL)
	{
		lst->head = new_token;
		lst->tail = new_token;
	}
	else
	{
		lst->tail->next = new_token;
		lst->tail = new_token;
	}
	return (new_token);
}

/* Returns the name of a token type (for debugging). */
char	*get_type_name(int type)
{
	if (type == WORD)
		return ("WORD");
	if (type == PIPE)
		return ("PIPE");
	if (type == REDIRECT_IN)
		return ("REDIRECT_IN");
	if (type == REDIRECT_OUT)
		return ("REDIRECT_OUT");
	if (type == APPEND_OUT)
		return ("APPEND_OUT");
	if (type == HEREDOC)
		return ("HEREDOC");
	else
		return ("UNKNOWN");
}

/* Prints a list of tokens (for debugging). */
void	print_tokens(t_token *head)
{
	t_token	*curr;

	curr = head;
	while (curr != NULL)
	{
		printf("[TYPE: %s, VALUE: \"%s\"] -> \n", get_type_name(curr->type),
			curr->value);
		curr = curr->next;
	}
	printf("NULL\n");
}

/* Frees the memory of a token list. */
void	free_tokens(t_token *head)
{
	t_token	*curr;
	t_token	*next_node;

	curr = head;
	while (curr != NULL)
	{
		next_node = curr->next;
		free(curr->value);
		free(curr);
		curr = next_node;
	}
}
