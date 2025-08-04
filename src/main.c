/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: preltien <preltien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 17:44:19 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/04 14:30:58 by preltien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_whitespace(char *str)
{
	while (*str)
	{
		if (*str != ' ' && *str != '\t')
			return (0);
		str++;
	}
	return (1);
}

void	init_shell_state(t_shell *state)
{
	extern char	**environ;
	int			len;

	len = envp_len(environ);
	state->envp = malloc(sizeof(char *) * (len + 1));
	if (!state->envp)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	if (duplicate_env(environ, state->envp, len) != 0)
	{
		perror("Erreur lors de la duplication de l'environnement");
		exit(EXIT_FAILURE);
	}
	state->exit_code = 0;
}

static void	process_line(char *line, t_shell *shell, t_token **tokens,
		t_command **commands)
{
	t_token	*tmp;

	if (*line && !is_whitespace(line))
	{
		*tokens = lexer(line);
		tmp = *tokens;
		while (tmp)
			tmp = tmp->next;
		if (*tokens && expand_token(*tokens, shell) != 0)
		{
			free_tokens(*tokens);
			*tokens = NULL;
		}
		if (*tokens)
			*commands = parser(*tokens);
	}
}

static void	shell_loop(t_shell *shell)
{
	char		*line;
	t_token		*tokens;
	t_command	*commands;

	while (1)
	{
		tokens = NULL;
		commands = NULL;
		line = readline("minishell> ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (*line)
			add_history(line);
		process_line(line, shell, &tokens, &commands);
		free(line);
		free_tokens(tokens);
		if (commands)
			shell->exit_code = execute(commands, shell);
		free_commands(commands);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;

	(void)ac;
	(void)av;
	(void)envp;
	init_shell_state(&shell);
	shell_loop(&shell);
	rl_clear_history();
	ft_free_array(shell.envp);
	return (shell.exit_code);
}
