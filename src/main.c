/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:51:14 by preltien          #+#    #+#             */
/*   Updated: 2025/08/21 19:14:03 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Parses and executes a given command line. */
static void	parse_and_execute(t_shell *state, char *line)
{
	t_token		*tokens;
	t_command	*commands;

	tokens = lexer(line);
	commands = NULL;
	if (tokens && expand_token(tokens, state) == 0)
		commands = parser(tokens);
	if (commands != NULL)
	{
		if (commands->argv && commands->argv[0] && ft_strcmp(commands->argv[0],
				"exit") == 0)
		{
			execute_builtin(commands->argv, state);
		}
		else
			state->exit_code = execute(commands, state);
	}
	free_tokens(tokens);
	free_commands(commands);
}

/* Checks if a string consists only of whitespace characters. */
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

/* Reads, parses, executes, and cleans up a single line of input. */
static int	process_line(t_shell *state)
{
	char	*line;

	line = read_line_input();
	if (line == NULL)
	{
		state->should_exit = true;
		return (0);
	}
	if (line[0] != '\0' && !is_whitespace(line))
	{
		if (isatty(STDIN_FILENO))
			add_history(line);
		parse_and_execute(state, line);
	}
	free(line);
	return (0);
}

/* Initialises the main shell structure (t_shell). */
static void	init_shell_state(t_shell *state, char **envp)
{
	state->envp = create_env_copy(envp);
	if (!state->envp)
	{
		perror("minishell: malloc error during env copy");
		exit(EXIT_FAILURE);
	}
	state->exit_code = 0;
	state->should_exit = false;
}

/* Initialises the shell and starts the main loop. */
int	main(int ac, char **av, char **envp)
{
	t_shell	shell_state;

	(void)ac;
	(void)av;
	if (isatty(STDIN_FILENO))
		display_title();
	setup_interactive_signals();
	init_shell_state(&shell_state, envp);
	while (shell_state.should_exit == false)
	{
		process_line(&shell_state);
	}
	ft_free_array(shell_state.envp);
	rl_clear_history();
	return (shell_state.exit_code);
}
