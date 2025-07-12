/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 17:44:19 by erazumov          #+#    #+#             */
/*   Updated: 2025/07/12 15:08:27 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_line(t_shell *state);
static int	is_whitespace(char *str);
static void	init_shell_state(t_shell *state, char **envp);

// Juste initialiser le shell et lancer la boucle qui appelle l'autre fonction
int	main(int ac, char **av, char **envp)
{
	t_shell		shell_state;

	(void)ac;
	(void)av;
	init_shell_state(&shell_state, envp);
	while (1)
	{
		if (process_line(&shell_state) != 0)
			break ;
	}
	rl_clear_history();
	return (shell_state.exit_code);
}

// La lire, l'analyser, l'exécuter, et la nettoyer
static int	process_line(t_shell *state)
{
	char		*line;
	t_token		*tokens;
	t_command	*commands;

	line = readline("minishell> ");
	if (line == NULL)
	{
		printf("exit\n");
		return (1);
	}
	if (*line)
		add_history(line);
	tokens = NULL;
	commands = NULL;
	if (*line != '\0' && !is_whitespace(line))
	{
		tokens = lexer(line);
		if (tokens && expand_token(tokens, state) == 0)
			commands = parser(tokens);
	}
	free(line);
	free_tokens(tokens);
	free_commands(commands);
	return (0);
}

// Vérifie si une chaîne de caractères est composée d'espaces et de tabs
static int	is_whitespace(char *str)
{
	while (*str)
	{
		if (*str != ' ' && *str != '\t')
			return (0);
		str++;
	}
	return (1);
}

// Initialise la structure principale du shell (t_shell)
static void	init_shell_state(t_shell *state, char **envp)
{
	state->envp = envp;
	state->exit_code = 0;
}

int	main(int ac, char **av, char **envp)
{
	char		*line;
	t_shell		shell_state;
	t_token		*tokens;
	t_command	*commands;

	(void)ac;
	(void)av;
	init_shell_state(&shell_state, envp);
	while (1)
	{
		tokens = NULL;
		commands = NULL;
		line = readline("minishell> ");
		if (line == NULL)
		{
			printf("exit\n");
			break ;
		}
		if (line && *line)
			add_history(line);
		if (*line != '\0' && !is_whitespace(line))
		{
			tokens = lexer(line);
			if (tokens && expand_token(tokens, &shell_state) != 0)
			{
				free_tokens(tokens);
				tokens = NULL;
			}
			if (tokens)
				commands = parser(tokens);
		}
		free(line);
		free_tokens(tokens);
		if (commands != NULL)
		{
			// shell_state.exit_code = execute(commands, &shell_state);
		}
		free_commands(commands);
	}
	rl_clear_history();
	return (shell_state.exit_code);
}

/*
void	handle_sigint(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}
*/
