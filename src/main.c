/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 17:44:19 by erazumov          #+#    #+#             */
/*   Updated: 2025/06/12 18:19:53 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
# include <readline/history.h>
#include <readline/readline.h>
//#include "../include/minishell.h"

int	main(void)
{
	char	*line;
	// Configure le gestionnaire de signal pour Ctrl+C
	// signal(SIGINT, handle_sigint);
	// Ignorer Ctrl+\ (SIGQUIT) pendant la saisie du prompt
	// signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		// 1. Afficher le prompt et lire la ligne
		line = readline("minishell> ");
		// 2. Gérer Ctrl+D (EOF) : readline retourne NULL
		if (line == NULL)
		{
			printf("exit\n"); // Affiche "exit" comme bash
			break ;
		}
		// 3. Gérer les lignes vides ou avec seulement des espaces
		if (*line == '\0') //|| is_whitespace(line))
		// is_whitespace est une fonction à implémenter pour vérifier si la ligne est vide ou contient seulement des espaces
		{
			free(line);
			continue ;
		}
		// 4. Ajouter la ligne à l'historique (pour les flèches haut/bas)
		if (line && *line)
			add_history(line);
		// --- POINT D'INTÉGRATION AVEC TON PARSER ---
		// Ici viendront les appels à tes fonctions de lexing/parsing
		printf("Tu as tapé : '%s'\n", line); // Pour le moment, juste un echo
		free(line);
	}
	rl_clear_history();
	return (0);
}

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
