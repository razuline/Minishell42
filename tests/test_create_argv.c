/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_create_argv.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 13:27:00 by erazumov          #+#    #+#             */
/*   Updated: 2025/06/21 13:36:45 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// cc -g tests/test_create_argv.c src/parser/parser_utils.c -Iinclude
// -Ilibft/include -Llibft -lft -o test_create_argv

void	print_argv(char **argv)
{
	int	i;

	printf("Contenu du tableau argv :\n");
	if (argv == NULL)
	{
		printf("  (null)\n");
		return ;
	}
	i = 0;
	while (argv[i] != NULL)
	{
		printf("  argv[%d] = \"%s\"\n", i, argv[i]);
		i++;
	}
	printf("  argv[%d] = (null)\n", i);
	printf("---------------------------\n");
}

int	main(void)
{
	char	**my_argv;

	my_argv = NULL;
	printf("État initial :\n");
	print_argv(my_argv);

	printf("Après ajout de \"ls\" :\n");
	my_argv = create_argv(my_argv, "ls");
	print_argv(my_argv); // On vérifie le résultat

	printf("Après ajout de \"-l\" :\n");
	my_argv = create_argv(my_argv, "-l");
	print_argv(my_argv); // On vérifie le résultat

	printf("Après ajout de \"/tmp\" :\n");
	my_argv = create_argv(my_argv, "/tmp");
	print_argv(my_argv); // On vérifie le résultat

	printf("Libération de la mémoire...\n");
	free(my_argv);
	printf("Mémoire libérée.\n");

	return (0);
}
