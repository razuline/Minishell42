/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 15:28:17 by preltien          #+#    #+#             */
/*   Updated: 2025/08/09 16:24:17 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_path(const char *cmd);
static char	*get_path_from_env(t_shell *state);
static char	*find_exec_in_paths(char *cmd, char **paths);

/* Trouve le chemin absolu d'une commande et met à jour argv[0]. */
void	get_absolute_path(char **argv, t_shell *state)
{
	char	*path_env;
	char	**paths;
	char	*exec_path;

	if (!argv || !argv[0] || !argv[0][0])
		return ;
	if (is_path(argv[0]))
		return ;
	path_env = get_path_from_env(state);
	if (!path_env)
		return ;
	paths = ft_split(path_env, ':');
	free(path_env);
	if (!paths)
		return ;
	exec_path = find_exec_in_paths(argv[0], paths);
	ft_free_array(paths);
	if (exec_path)
	{
		free(argv[0]);
		argv[0] = exec_path;
	}
}

/* Vérifie si une commande est déjà un chemin (contient un '/'). */
static int	is_path(const char *cmd)
{
	return (ft_strchr(cmd, '/') != NULL);
}

/* Récupère la variable d'environnement PATH. */
static char	*get_path_from_env(t_shell *state)
{
	int	i;

	i = 0;
	while (state->envp && state->envp[i])
	{
		if (ft_strncmp(state->envp[i], "PATH=", 5) == 0)
			return (ft_strdup(state->envp[i] + 5));
		i++;
	}
	return (NULL);
}

/* Cherche un exécutable dans les répertoires listés dans PATH. */
static char	*find_exec_in_paths(char *cmd, char **paths)
{
	int		i;
	char	*tmp_path;
	char	*full_path;

	i = 0;
	while (paths && paths[i])
	{
		tmp_path = ft_strjoin(paths[i], "/");
		if (!tmp_path)
			return (NULL);
		full_path = ft_strjoin(tmp_path, cmd);
		free(tmp_path);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}
