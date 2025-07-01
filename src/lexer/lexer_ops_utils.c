/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_ops_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 13:40:58 by erazumov          #+#    #+#             */
/*   Updated: 2025/07/01 13:41:32 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_single_token(t_token **head, t_token **tail, char **c)
{
	if (**c == '|')
	{
		create_token(head, tail, "|", PIPE);
		*c += 1;
		return (1);
	}
	else if (**c == '<')
	{
		create_token(head, tail, "<", REDIRECT_IN);
		*c += 1;
		return (1);
	}
	else if (**c == '>')
	{
		create_token(head, tail, ">", REDIRECT_OUT);
		*c += 1;
		return (1);
	}
	return (0);
}

int	ft_double_token(t_token **head, t_token **tail, char **c)
{
	if (**c == '<' && *(*c + 1) == '<')
	{
		create_token(head, tail, "<<", HEREDOC);
		*c += 2;
		return (1);
	}
	else if (**c == '>' && *(*c + 1) == '>')
	{
		create_token(head, tail, ">>", APPEND_OUT);
		*c += 2;
		return (1);
	}
	return (0);
}
