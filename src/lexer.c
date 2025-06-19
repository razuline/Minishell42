/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 13:14:07 by erazumov          #+#    #+#             */
/*   Updated: 2025/06/19 16:20:07 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	word_handle(t_token **head, t_token **tail, char **curr)
{
	int	len;
	char	*start;
	char	*end;
	char	*extract_word;
	char	*clean_word;

	start = *curr;
	end = ft_word_end(start);
	len = end - start;
	if (len == 0)
	{
		*curr = end;
		return (0);
	}
	extract_word = ft_substr(start, 0, len);
}

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
	char	*get_word;
	t_token	*start;
	t_token	*end;

	start = NULL;
	end = NULL;
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
				create_token(&start, &end, "||", OR_OPERATOR);
				curr_char += 2;
			}
			else
			{
				create_token(&start, &end, "|", PIPE);
				curr_char++;
			}
		}
		else if (*curr_char == '>')
		{
			if (*(curr_char + 1) == '>')
			{
				create_token(&start, &end, ">>", APPEND_OUT);
				curr_char += 2;
			}
			else
			{
				create_token(&start, &end, ">", REDIRECT_OUT);
				curr_char++;
			}
		}
		else if (*curr_char == '<')
		{
			if (*(curr_char + 1) == '<')
			{
				create_token(&start, &end, "<<", HEREDOC);
				curr_char += 2;
			}
			else
			{
				create_token(&start, &end, "<", REDIRECT_IN);
				curr_char++;
			}
		}
		else if (*curr_char == ';')
		{
			create_token(&start, &end, ";", SEMICOLON);
			curr_char++;
		}
		else if (*curr_char == '(')
		{
			create_token(&start, &end, "(", PARENTHESIS_OPEN);
			curr_char++;
		}
		else if (*curr_char == ')')
		{
			create_token(&start, &end, ")", PARENTHESIS_CLOSE);
			curr_char++;
		}
		else
		{
			start = curr_char;
			end = ft_word_end(start);
			ft_delete_quotes(*line);
		}
	}
	print_token(start);
	free_token(start);
}

