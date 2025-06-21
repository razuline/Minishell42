/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_delete_quotes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 14:53:21 by erazumov          #+#    #+#             */
/*   Updated: 2025/06/21 14:58:03 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <string.h>

/* cc -g tests/test_delete_quotes.c src/lexer/lexer_word_utils.c -Iinclude
-Ilibft/include -Llibft -lft -o test_runner */
/* valgrind --leak-check=full ./test_runner */

#define GREEN "\033[32m"
#define RED   "\033[31m"
#define RESET "\033[0m"

void	run_test(const char *input, const char *expected_output)
{
	char	*actual_output;

	actual_output = ft_delete_quotes((char *)input);
	if (strcmp(actual_output, expected_output) == 0)
	{
		printf(GREEN "[OK] " RESET);
		printf("Input: \"%s\" -> Output: \"%s\"\n", input, actual_output);
	}
	else
	{
		printf(RED "[KO] " RESET);
		printf("Input: \"%s\" -> Output: \"%s\" | Attendu: \"%s\"\n",
			input, actual_output, expected_output);
	}
	free(actual_output);
}

int	main(void)
{
	printf("--- Lancement des tests unitaires pour ft_delete_quotes ---\n");

	// Cas nominaux
	run_test("\"salut\"", "salut");
	run_test("'hello world'", "hello world");
	run_test("\"\"", "");

	// Cas où rien ne doit changer
	run_test("pas de quotes", "pas de quotes");
	run_test("'pas ferme", "'pas ferme");
	run_test("\"mixte'", "\"mixte'");
	run_test("au'milieu'", "au'milieu'");

	// Cas limites
	run_test("'", "'");
	run_test("\"", "\"");
	run_test("", "");

	// Le cas NULL doit être testé séparément car strcmp va crasher
	char	*result_null = ft_delete_quotes(NULL);
	if (result_null == NULL)
		printf(GREEN "[OK] " RESET "Input: NULL -> Output: NULL\n");
	else
		printf(RED "[KO] " RESET "Input: NULL -> Output: Non-NULL\n");
	free(result_null); // free(NULL) ne fait rien, c'est sûr.

	printf("--- Tests terminés ---\n");
	return (0);
}
