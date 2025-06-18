/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 17:45:03 by erazumov          #+#    #+#             */
/*   Updated: 2025/06/18 15:06:34 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/include/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>

/******************************************************************************
*  STRUCTS																	  *
******************************************************************************/

typedef struct s_token
{
	char			*value;
	int				type;
	struct s_token	*next;
}					t_token;

typedef struct s_command
{
}					t_command;

/******************************************************************************
*  ENUMS																	  *
******************************************************************************/
enum				e_token_type
{
	WORD,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	APPEND_OUT,
	HEREDOC,
	OR_OPERATOR,
	SEMICOLON,
	PARENTHESIS_OPEN,
	PARENTHESIS_CLOSE,
	ENV_VAR
};

enum				e_quotes_type
{
	DEFAULT,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
};

/******************************************************************************
*  FUNCTIONS																  *
******************************************************************************/

/* -------------------------- LEXER -----------------------------------------*/





//void	handle_sigint(int sig);
int					is_whitespace(char *str);

#endif
