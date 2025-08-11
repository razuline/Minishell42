/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 11:04:24 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/11 11:29:56 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_received_signal = 0;

/* C'est le handler pour le mode interactif.
 * Il s'exécute quand l'utilisateur fait Ctrl-C au niveau du prompt.*/
void	signal_handler(int sig)
{
	g_received_signal = sig;
	if (sig == SIGINT)
	{
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

/* Règle les signaux pour le mode INTERACTIF (quand le shell attend
 * une commande). C'est le mode par défaut. */
void	setup_interactive_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

/* Règle les signaux pour le mode NON-INTERACTIF (quand une commande tourne).
 * Le shell parent ignorera les signaux pour laisser l'enfant les gérer. */
void	setup_non_interactive_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

/* Rétablit le comportement par défaut des signaux.
 * Doit être appelé par le processus ENFANT juste après le fork. */
void	setup_child_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}
