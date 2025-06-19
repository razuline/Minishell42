/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_quotes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 14:53:21 by erazumov          #+#    #+#             */
/*   Updated: 2025/06/19 15:33:35 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	run_test(char *input)
{
	char	*output;

	printf("Input	: [%s]\n", input);
	output = ft_delete_quotes(input);
	printf("Output   : [%s]\n", output);
	printf("----------\n");
	if (output)
		free(output);
}

int	main(void)
{
	printf("--- Lancement des tests pour ft_delete_quotes ---\n");

	// Cas nominaux (ce qui doit marcher normalement)
	run_test("\"salut\""); // -> [salut]
	run_test("'salut le monde'"); // -> [salut le monde]
	run_test("\"\""); // -> []
	run_test("''"); // -> []
	// Cas où il ne faut RIEN changer (la fonction doit juste dupliquer)
	run_test("salut"); // -> [salut]
	run_test("'salut\""); // -> ['salut"]
	run_test("sa'lu't"); // -> [sa'lu't]
	run_test("\"salut"); // -> ["salut]
	run_test("salut'"); // -> [salut']
	run_test("\"salut\"monde"); // -> ["salut"monde]
	// Cas limites (les cas "pièges")
	run_test(""); // -> []
	run_test("\""); // -> ["]
	run_test("'"); // -> [']
	run_test(NULL); // -> NULL

	printf("--- Tests terminés ---\n");
	return (0);
}
