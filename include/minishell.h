/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 17:45:03 by erazumov          #+#    #+#             */
/*   Updated: 2025/06/20 14:38:24 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>

/******************************************************************************
*  STRUCTS																		*
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
*  ENUMS																		*
******************************************************************************/
enum				e_token_type
{
	SPACES,
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

enum				e_quote_type
{
	DEFAULT,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
};

/******************************************************************************
*  FUNCTIONS																	*
******************************************************************************/

/* -------------------------- LEXER -----------------------------------------*/

/* lexer.c */
t_token				*lexer(char *line);
int					handle_token(t_token **head, t_token **tail, char **ch);
int					handle_word(t_token **head, t_token **tail, char **ch);

/* lexer_word_utils.c */
int					ft_token_type(char *word, int i);
int					ft_quote_type(char *word, int type, int i);
char				*ft_word_end(char *word);
char				*ft_delete_quotes(char *word);

/* lexer_token_utils.c */
t_token				*create_token(t_token **head, t_token **tail, char *word,
						int type);
t_token				*free_token(t_token *head);
t_token				*print_token(void);

//void	handle_sigint(int sig);
int					is_whitespace(char *str);

#endif
