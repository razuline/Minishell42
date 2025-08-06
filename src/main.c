/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:51:14 by preltien          #+#    #+#             */
/*   Updated: 2025/08/06 19:16:14 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_line(t_shell *state);
static void	parse_and_execute(t_shell *state, char *line);
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
	ft_free_array(shell_state.envp);
	return (shell_state.exit_code);
}

// La lire, l'analyser, l'exécuter, et la nettoyer
static int	process_line(t_shell *state)
{
	char		*line;

	line = readline("minishell> ");
	if (line == NULL)
	{
		printf("exit\n");
		return (1);
	}
	if (*line)
	parse_and_execute(state, line);
	free(line);
	return (0);
}

// Analyse et exécute une ligne de commande donnée
static void	parse_and_execute(t_shell *state, char *line)
{
	t_token		*tokens;
	t_command	*commands;

	if (*line == '\0' || is_whitespace(line))
		return ;
	add_history(line);
	tokens = NULL;
	commands = NULL;
	tokens = lexer(line);
	if (tokens && expand_token(tokens, state) == 0)
		commands = parser(tokens);
	if (commands != NULL)
		state->exit_code = execute(commands, state);
	free_tokens(tokens);
	free_commands(commands);
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
	int	len;

	len = envp_len(envp);
	state->envp = malloc(sizeof(char *) * (len + 1));
	if (!state->envp)
	{
		perror("minishell: malloc error");
		exit(EXIT_FAILURE);
	}
	if (duplicate_env(envp, state->envp, len) != 0)
	{
		free(state->envp);
		perror("minishell: malloc error during env copy");
		exit(EXIT_FAILURE);
	}
	state->envp[len] = NULL;
	state->exit_code = 0;
}
