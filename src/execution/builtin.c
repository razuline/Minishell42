/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: preltien <preltien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 15:00:00 by preltien          #+#    #+#             */
/*   Updated: 2025/07/11 16:58:31 by preltien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;

/*
 * echo : affiche les arguments, gestion simplifiée
 */
int	builtin_echo(char **argv)
{
	int i = 1;

	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	printf("\n");
	return (0);
}

/*
 * cd : change de répertoire
 */
int	builtin_cd(char **argv)
{
	char	*dir;
	int		ret;

	dir = argv[1];
	if (!dir)
		dir = getenv("HOME");
	if (!dir)
	{
		fprintf(stderr, "minishell: cd: HOME not set\n");
		return (1);
	}
	ret = chdir(dir);
	if (ret != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	return (0);
}

/*
 * pwd : affiche le répertoire courant
 */
int	builtin_pwd(void)
{
	char cwd[4096];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("minishell: pwd");
		return (1);
	}
	printf("%s\n", cwd);
	return (0);
}

/*
 * env : affiche les variables d'environnement
 */
int	builtin_env(char **envp)
{
	int i = 0;

	if (!envp)
		return (1);
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
	return (0);
}

/*
 * export : stub
 */
int	builtin_export(char **argv)
{
	(void)argv;
	printf("minishell: export: not implemented\n");
	return (0);
}

/*
 * unset : stub
 */
int	builtin_unset(char **argv)
{
	(void)argv;
	printf("minishell: unset: not implemented\n");
	return (0);
}

/*
 * exit : quitte le shell
 */
int	builtin_exit(char **argv)
{
	int	code = 0;

	if (argv[1])
		code = atoi(argv[1]);
	exit(code);
}
