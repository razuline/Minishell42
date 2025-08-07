/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: preltien <preltien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 17:45:03 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/07 10:31:00 by preltien         ###   ########.fr       */
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

// ************************************************************************** //
//                                 Structures                                 //
// ************************************************************************** //

typedef struct s_token
{
	char				*value;
	int					type;
	int					quote_type;
	struct s_token		*next;
}						t_token;

typedef struct s_token_lst
{
	t_token				*head;
	t_token				*tail;
}						t_token_lst;

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

typedef struct s_pipex_ctx
{
	t_command			*cmd;
	t_shell				*state;
	int					prev_fd;
	int					pipe_fd[2];
	int					is_last;
}						t_pipex_ctx;

// ************************************************************************** //
//                                Enumerations                                //
// ************************************************************************** //

enum					e_token_type
{
	WORD = 0,
	PIPE = 1,
	REDIRECT_IN = 2,
	REDIRECT_OUT = 3,
	APPEND_OUT = 4,
	HEREDOC = 5,
	ENV_VAR = 6
};

typedef enum e_quote_type
{
	DEFAULT,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
}						t_quote_type;

// ************************************************************************** //
//                                 Functions                                  //
// ************************************************************************** //

/* LEXER -------------------------------------------------------------------- */

/* lexer.c */
t_token					*lexer(char *line);

/* lexer_handlers.c */
int						handle_word(t_token_lst *lst, char **c);
int						handle_single_op(t_token_lst *lst, char **c);
int						handle_double_op(t_token_lst *lst, char **c);

/* lexer_word_utils.c */
int						update_quote_state(char *word, int type, int i);
int						get_quote_type(char *word);
char					*find_word_end(char *word);
char					*remove_quotes_from_word(char *word);

/* lexer_token_utils.c */
t_token					*create_token(t_token_lst *lst, char *word, int type,
							int quote_info);
char					*get_type_name(int type);
void					print_tokens(t_token *head);
void					free_tokens(t_token *head);

/* EXPANSION ---------------------------------------------------------------- */

/* expansion.c */
int						expand_token(t_token *head, t_shell *state);

/* expansion_len_utils.c */
size_t					calculate_expanded_len(const char *value,
							t_shell *state);

/* expansion_var_utils.c */
char					*get_var_name(const char *input, int *i_ptr);

/* expansion_append_utils.c */
size_t					append_str_to_result(char *dest, const char *src,
							size_t j);

/* PARSER --------------------------------------------------------------------*/

/* parser.c */
t_command				*parser(t_token *head);

/* parser_utils.c */
t_command				*create_command(void);
char					**create_argv(char **old_argv, char *new_str);
void					add_redir_to_cmd(t_command *cmd, t_redir *new_redir);

/* parser_free_utils.c */
void					free_commands(t_command *cmd_head);

/* parser_print_utils.c */
int						is_redir_token(int type);
void					print_commands(t_command *cmd_head);

/* EXECUTION ---------------------------------------------------------------- */

/* execution.c */
int						exec_cmd(t_command *cmd, t_shell *state);
int						execute(t_command *cmds, t_shell *state);

/* execution_utils.c */
int						is_builtin(char *cmd);
int						exec_builtin(char **args, t_shell *state);
void					ft_free_array(char **array);
int						is_valid_varname(char *name);
int						is_directory(const char *path);

/* builtin.c */
int						builtin_echo(char **argv);
int						builtin_cd(char **argv);
int						builtin_pwd(void);
int						builtin_env(char **envp);
int						builtin_exit(char **argv);

/*pipex.c*/
void					pipex_close_fds(int *prev_fd, int pipe_fd[2],
							int is_last);
int						pipex_exec_loop(t_command *cmds, t_shell *state);
int						has_pipe(t_command *cmds);
int						create_pipe_if_needed(int pipe_fd[2], int is_last);
void					pipex_fork_and_exec(t_pipex_ctx *ctx);
int						fork_and_handle_child(t_pipex_ctx *ctx);
void					wait_for_child(pid_t pid, t_shell *state);

/*redir*/
int						here_document(const char *limiter);
int						apply_redirections(t_redir *redir);

/*export*/
int						builtin_export(t_shell *state, char **argv);

/*unset*/
int						process_unset_arg(t_shell *state, char *arg);
int						builtin_unset(char **argv, t_shell *state);
int						remove_env_var(char ***envp, const char *var);

/*get_path*/
char					*get_path_from_env(t_shell *state);
char					*find_executable_path(char *cmd, char **path_split);
void					get_absolute_path(char **cmd, t_shell *state);

/*env*/
int						envp_add_entry(char ***envp, char *entry);
void					print_env(t_shell *state);
int						set_env_var(t_shell *state, const char *key,
							const char *value);
int						duplicate_env(char **src, char **dst, int count);
int						envp_len(char **envp);

#endif
