/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   final_test.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:38:42 by erazumov          #+#    #+#             */
/*   Updated: 2025/06/26 14:54:17 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	int			i;
	t_token		*tokens;
	t_command	*command_list;

	const char	*test_lines[] = {
		// Test 1 : Commande simple
		"ls -l -a",
		// Test 2 : Quotes simples
		"echo 'salut le monde'",
		// Test 3 : Quotes doubles
		"echo \"hello world\"",
		// Test 4 : Pipe simple
		"cat Makefile | grep gcc",
		// Test 5 : Redirection simple
		"grep minishell < libft/ft_isalpha.c",
		// Test 6 : Redirection de sortie
		"ls > output.txt",
		// Test 7 : Combinaison complète
		"< infile grep '42' | wc -l >> log.txt",
		// Test 8 : Commande complexe avec plusieurs arguments et pipes
		"cat -e file1 | grep -i 'test' | wc -w",
		// Test 9 : Redirections avant la commande
		">> append.log cat",
		// Test 10 : Ligne vide ou avec des espaces
		"    ",
		// Test 11 : Erreur de syntaxe
		"ls > | wc",
		// Marqueur de fin de tableau
		NULL};
	i = 0;
	while (test_lines[i] != NULL)
	{
		printf("\n====================================================\n");
		printf("TEST N°%d: Ligne = \"%s\"\n", i + 1, test_lines[i]);
		printf("====================================================\n");
		// ÉTAPE 1 : Appeler le LEXER pour transformer la ligne en tokens.
		tokens = lexer((char *)test_lines[i]);
		// ÉTAPE 2 : Donner le résultat du lexer (la liste de tokens) au PARSER.
		command_list = parser(tokens);
		// ÉTAPE 3 : Afficher le résultat final du parser.
		if (command_list == NULL)
		{
			printf("--> Le parser a retourné NULL (ligne vide ou erreur de syntaxe).\n");
		}
		else
		{
			printf("--> Résultat du parsing :\n");
			print_commands(command_list);
		}
		// ÉTAPE 4 : Libérer la mémoire des DEUX listes.
		free_tokens(tokens);
		free_commands(command_list);
		i++;
	}
	return (0);
}
