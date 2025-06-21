/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_word_end.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 11:32:06 by erazumov          #+#    #+#             */
/*   Updated: 2025/06/21 12:41:54 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <string.h>

// cc -g tests/test_word_end.c src/lexer/lexer_word_utils.c -Iinclude
// -Ilibft/include -Llibft -lft -o test_word_end_runner

#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

void	run_test_word_end(const char *input, char expected_char_at_end)
{
	char	*result_pointer;

	result_pointer = ft_word_end((char *)input);
	if (result_pointer == NULL)
	{
		if (expected_char_at_end == '\0')
		{
			printf(GREEN "[OK] " RESET);
			printf("Input: NULL -> Output: NULL\n");
		}
		else
		{
			printf(RED "[KO] " RESET);
			printf("Input: \"%s\" -> Output: NULL | Attendu: '%c'\n",
					input, expected_char_at_end);
		}
		return ;
	}
	if (*result_pointer == expected_char_at_end)
	{
		printf(GREEN "[OK] " RESET);
		printf("Input: \"%s\" -> s'est bien arrêté sur le caractère\
			'%c'\n", input, *result_pointer);
	}
	else
	{
		printf(RED "[KO] " RESET);
		printf("Input: \"%s\" -> s'est arrêté sur '%c'\
			| Mais on attendait '%c'\n", input, *result_pointer,
			expected_char_at_end);
	}
}

int	main(void)
{
	printf("--- Début des tests pour ft_word_end ---\n");
	// Test 1: Mot simple avant un espace
	run_test_word_end("echo salut", ' ');
	// Test 2: Mot simple avant un opérateur
	run_test_word_end("cat|grep", '|');
	// Test 3: Mot à la fin de la ligne (doit s'arrêter sur le caractère nul '\0')
	run_test_word_end("word", '\0');
	// Test 4: Mot entre guillemets (doit ignorer l'espace et s'arrêter au pipe)
	run_test_word_end("'hello world'|pipe", '|');
	// Test 5: Guillemet non fermé (doit aller jusqu'à la fin de la ligne)
	run_test_word_end("\"unclosed word", '\0');
	// Test 6 (Optionnel mais recommandé): Input NULL
	// Pour ce cas spécial, on peut utiliser '\0' comme caractère attendu
	// pour notre test
	run_test_word_end(NULL, '\0');
	printf("--- Fin des tests ---\n");
	return (0);
}
