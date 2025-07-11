/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: preltien <preltien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 17:45:03 by erazumov          #+#    #+#             */
/*   Updated: 2025/07/11 14:38:39 by preltien         ###   ########.fr       */
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
	int					quote_type;
	struct s_token		*next;
}						t_token;

typedef struct s_tokenlist
{
	t_token				**head;
	t_token				**tail;
}						t_tokenlist;


typedef struct s_redir
{
	int					type;
	char				*file;
	struct s_redir		*next;
}						t_redir;

typedef struct s_command
{
	char				**argv;
	t_redir				*redir;
	struct s_command	*next;
}						t_command;

typedef struct s_shell_state
{
	char				**envp;
	int					exit_code;
}						t_shell;

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


/* lexer_word.c */
int						ft_delimiter(char c);
int						ft_quote_type(char *word, int type, int i);
char					*ft_word_end(char *word);
char					*ft_delete_quotes(char *word);

/* lexer_word_utils.c */
int						handle_word(t_tokenlist *lst, char **ch);


/* lexer_ops_utils.c */
int						ft_single_token(t_tokenlist *lst, char **c);
int						ft_double_token(t_tokenlist *lst, char **c);

/* lexer_token_utils.c */
t_token					*create_token(t_tokenlist *lst, char *word, int type, int quote_info);
char					*get_type_name(int type);
void					print_tokens(t_token *head);
void					free_tokens(t_token *head);

/* -------------------------- EXPANSION --------------------------------------*/

/* expansion.c */
int						expand_token(t_token *head, t_shell *state);

/* expansion_var_utils.c */
int						append_env_var(char **res_ptr, const char *input,
							int *i_ptr);

/* expansion_append_utils.c */
int						append_char(char **res_ptr, char c);
int						append_dollar(char **res_ptr);
int						append_exit_status(char **res_ptr, t_shell *state);

/* -------------------------- PARSER -----------------------------------------*/

/* parser.c */
t_command				*parser(t_token *head);

/* parser_utils.c */
t_command				*create_command(void);
char					**create_argv(char **old_argv, char *new_str);
void					add_redir_to_cmd(t_command *cmd, t_redir *new_redir);

/* parser_print_utils.c */
int						ft_redirection(int type);
void					print_commands(t_command *cmd_head);

/* parser_free_utils.c */
void					free_commands(t_command *cmd_head);

/* -------------------------- EXECUTION --------------------------------------*/

/* execution.c */
int						execute(t_command *cmd_lst, t_shell *state);
char					*find_cmd_path(char *name, char **envp);


#endif
