/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: preltien <preltien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 15:00:00 by preltien          #+#    #+#             */
/*   Updated: 2025/08/05 15:33:13 by preltien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern char	**environ;

/*
 * echo : affiche les arguments, gestion simplifiée
 */
int	builtin_echo(char **argv)
{
	int	i;

	i = 1;
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

	if (argv[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	dir = argv[1];
	if (!dir)
		dir = getenv("HOME");
	if (!dir)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
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
	char	cwd[4096];

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
	int	i;

	i = 0;
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
 * exit : quitte le shell
 */

int	builtin_exit(char **argv)
{
	int	error;
	int	exit_code;

	write(1, "exit\n", 5);
	if (!argv[1])
		exit(0);
	exit_code = ft_atoi_strict(argv[1], &error);
	if (error)
	{
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n",
			argv[1]);
		exit(2);
	}
	if (argv[2])
	{
		fprintf(stderr, "minishell: exit: too many arguments\n");
		return (1);
	}
	exit(exit_code);
}
