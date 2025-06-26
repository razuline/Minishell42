/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 16:25:54 by erazumov          #+#    #+#             */
/*   Updated: 2025/06/26 17:53:59 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	append_exit_status(char **res_ptr, t_shell *state)
{
	char	*exit_code_str;
	char	*tmp;

	exit_code_str = ft_itoa(state->exit_code);
	if (!exit_code_str)
		return (1);
	tmp = *res_ptr;
	*res_ptr = ft_strjoin(tmp, exit_code_str);
	free(tmp);
	free(exit_code_str);
	if (!*res_ptr)
		return (1);
	return (0);
}

static int	append_env_var(char **res_ptr, const char *input, int *i_ptr)
{
	int		len;

	len = 0;
	while (ft_isalnum(input[*i_ptr + len]) || input[*i_ptr + len] == '_' )
		len++;
	if (len == 0)
	{

	}
}

static char	*expand_str(const char *value, t_shell *state)
{
	char	*result;
	char	*expanded;
	int		i;

	if (!value)
		return (NULL);
	result = ft_strdup("");
	i = 0;
	while (value[i])
	{
		if (value[i] == '$' && value[i + 1] != '\0' && !ft_isspace(value[i
				+ 1]))
		{
			i++;
			if (value[i] == '?')
			{
				expanded = ft_itoa(state->exit_code);
				ft_strjoin(result, expanded);
				free(expanded);
				i++;
			}
			else
			{
			}
		}
	}
	return (result);
}
