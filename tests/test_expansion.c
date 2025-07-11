/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_expansion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 13:06:55 by erazumov          #+#    #+#             */
/*   Updated: 2025/06/27 15:02:56 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* cc -g tests/test_expansion.c src/lexer/*.c src/expansion/*.c 
src/parser/parser_utils.c -Iinclude -Ilibft/include -Llibft -lft -o 
test_expansion_runner */
/* leaks --atExit -- ./test_expansion_runner */

int	main(int ac, char **av, char **envp)
{
	t_shell		shell_state;
	const char	*test_lines[] = {
		"echo $?",
		"echo $USER",
		"echo 'ne pas expandre $USER'",
		"echo \"expandre $USER\"",
		"echo salut-$USER",
		NULL
	};
	int			i = 0;

	// 1. Initialiser l'état du shell (très important)
	shell_state.envp = envp; // Ou une copie si tu préfères
	shell_state.exit_code = 42; // On met une valeur pour tester $?

	printf("--- Début des tests pour l'Expansion ---\n");
	while (test_lines[i] != NULL)
	{
		printf("\n====================================\n");
		printf("TEST N°%d: Ligne = \"%s\"\n", i + 1, test_lines[i]);
		printf("------------------------------------\n");

		// a. On passe la ligne au Lexer pour obtenir les tokens bruts
		t_token	*tokens = lexer((char *)test_lines[i]);
		printf("Tokens AVANT expansion :\n");
		print_tokens(tokens);

		// b. On passe la liste de tokens à l'Expander
		int	status = expand_token(tokens, &shell_state);

		if (status != 0)
			printf("ERREUR PENDANT L'EXPANSION\n");
		else
		{
			// c. On affiche le résultat pour vérifier
			printf("\nTokens APRES expansion :\n");
			print_tokens(tokens);
		}
		
		// d. On nettoie
		free_tokens(tokens);
		i++;
	}
	return (0);
}
