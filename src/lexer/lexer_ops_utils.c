/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_ops_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: preltien <preltien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 13:40:58 by erazumov          #+#    #+#             */
/*   Updated: 2025/07/13 15:22:13 by preltien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_single_token(t_tokenlist *lst, char **c)
{
	if (**c == '|')
	{
		create_token(lst, "|", PIPE, DEFAULT);
		*c += 1;
		return (1);
	}
	else if (**c == '<')
	{
		create_token(lst, "<", REDIRECT_IN, DEFAULT);
		*c += 1;
		return (1);
	}
	else if (**c == '>')
	{
		create_token(lst, ">", REDIRECT_OUT, DEFAULT);
		*c += 1;
		return (1);
	}
	return (0);
}

int	ft_double_token(t_tokenlist *lst, char **c)
{
	if (**c == '<' && *(*c + 1) == '<')
	{
		create_token(lst, "<<", HEREDOC, DEFAULT);
		*c += 2;
		return (1);
	}
	else if (**c == '>' && *(*c + 1) == '>')
	{
		create_token(lst, ">>", APPEND_OUT, DEFAULT);
		*c += 2;
		return (1);
	}
	return (0);
}