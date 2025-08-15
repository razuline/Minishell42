/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 13:57:01 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/15 14:35:31 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define BOLD_WHITE "\033[1;37m"
#define BOLD_RED "\033[1;31m"
#define RESET "\033[0m"

/* Builds the dynamic prompt string. */
char	*build_prompt(void)
{
	const char	*username;
	char		*prompt;
	size_t		len;

	username = getenv("USER");
	if (!username)
		username = "user";
	len = ft_strlen(BOLD_WHITE) + ft_strlen(username) + ft_strlen(RESET) \
		+ ft_strlen(BOLD_RED) + ft_strlen("@") + ft_strlen(RESET) \
		+ ft_strlen(BOLD_RED) + ft_strlen("minishell> ") + ft_strlen(RESET);
	prompt = malloc(sizeof(char) * (len + 1));
	if (!prompt)
		return (NULL);
	ft_strcpy(prompt, BOLD_WHITE);
	ft_strcat(prompt, (char *)username);
	ft_strcat(prompt, RESET);
	ft_strcat(prompt, BOLD_RED);
	ft_strcat(prompt, "@");
	ft_strcat(prompt, "minishell> ");
	ft_strcat(prompt, RESET);
	return (prompt);
}
