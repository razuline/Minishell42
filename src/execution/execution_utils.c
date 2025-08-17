/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 14:35:16 by erazumov          #+#    #+#             */
/*   Updated: 2025/08/17 17:44:26 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Vérifie si un nom de commande correspond à un built-in. */
int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

/* Vérifie si un chemin correspond à un répertoire. */
int	is_directory(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
		return (0);
	return (S_ISDIR(path_stat.st_mode));
}

/* Aiguille vers la bonne fonction built-in et retourne son code de sortie. */
int	execute_builtin(char **argv, t_shell *state)
{
	if (ft_strcmp(argv[0], "echo") == 0)
		return (builtin_echo(argv));
	if (ft_strcmp(argv[0], "cd") == 0)
		return (builtin_cd(argv, state));
	if (ft_strcmp(argv[0], "pwd") == 0)
		return (builtin_pwd());
	if (ft_strcmp(argv[0], "env") == 0)
		return (builtin_env(state));
	if (ft_strcmp(argv[0], "export") == 0)
		return (builtin_export(argv, state));
	if (ft_strcmp(argv[0], "unset") == 0)
		return (builtin_unset(argv, state));
	if (ft_strcmp(argv[0], "exit") == 0)
		return (builtin_exit(argv, state));
	return (1);
}

/* Helper pour sauvegarder les descripteurs de fichiers standard
 * (stdin et stdout). Retourne 0 en cas de succès, -1 en cas d'erreur. */
int	save_original_fds(int *stdin_save, int *stdout_save)
{
	*stdin_save = dup(STDIN_FILENO);
	if (*stdin_save == -1)
	{
		perror("minishell: dup");
		return (-1);
	}
	*stdout_save = dup(STDOUT_FILENO);
	if (*stdout_save == -1)
	{
		close(*stdin_save);
		perror("minishell: dup");
		return (-1);
	}
	return (0);
}

/* Helper pour restaurer les descripteurs de fichiers standard sauvegardés. */
int	restore_original_fds(int stdin_save, int stdout_save)
{
	if (dup2(stdin_save, STDIN_FILENO) == -1
		|| dup2(stdout_save, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(stdin_save);
		close(stdout_save);
		return (-1);
	}
	close(stdin_save);
	close(stdout_save);
	return (0);
}
