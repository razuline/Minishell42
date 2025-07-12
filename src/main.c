/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: preltien <preltien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 17:44:19 by erazumov          #+#    #+#             */
/*   Updated: 2025/07/11 11:08:33 by preltien         ###   ########.fr       */
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

void	init_shell_state(t_shell *state, char **envp)
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
