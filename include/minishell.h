/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 17:45:03 by erazumov          #+#    #+#             */
/*   Updated: 2025/06/21 18:03:30 by erazumov         ###   ########.fr       */
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
	char				*value;
	int					type;
	struct s_token		*next;
}						t_token;

typedef struct s_redir
{
	int					type;
	char				*filename;
	struct s_redir		*next;
}						t_redir;

typedef struct s_command
{
	char				**argv;
	t_redir				*redir;
	struct s_command	*next;
}						t_command;

/******************************************************************************
*  ENUMS																		*
******************************************************************************/
enum					e_token_type
{
	WORD,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	APPEND_OUT,
	HEREDOC,
	ENV_VAR
};

enum					e_quote_type
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
t_token					*lexer(char *line);
int						ft_single_token(t_token **head, t_token **tail,
							char **c);
int						ft_double_token(t_token **head, t_token **tail,
							char **c);
int						handle_word(t_token **head, t_token **tail, char **ch);

/* lexer_word_utils.c */
int						ft_delimiter(char c);
int						ft_quote_type(char *word, int type, int i);
char					*ft_word_end(char *word);
char					*ft_delete_quotes(char *word);

/* lexer_token_utils.c */
t_token					*create_token(t_token **head, t_token **tail,
							char *word, int type);
void					print_tokens(t_token *head);
void					free_tokens(t_token *head);

/* -------------------------- PARSER -----------------------------------------*/

/* lexer.c */
t_command				*parser(t_token *head);

/* lexer_utils.c */
char					**create_argv(char **old_argv, char *new_str);

//void	handle_sigint(int sig);
int						is_whitespace(char *str);

#endif
