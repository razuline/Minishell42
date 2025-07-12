/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: preltien <preltien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 10:39:58 by erazumov          #+#    #+#             */
/*   Updated: 2025/07/11 11:31:22 by preltien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*check_dir_path(char *cmd_name);
static char	*search_in_path(char *cmd_name, char **path_dir);

static void free_path_dirs(char **paths)
{
    int i = 0;

    if (!paths)
        return;
    while (paths[i])
    {
        free(paths[i]);
        i++;
    }
    free(paths);
}

char	*find_cmd_path(char *name, char **envp)
{
	int		i;
	char	*path;
	char	**path_dirs;

	path = check_dir_path(name);
	if (path)
		return (path);
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (envp[i] == NULL)
		return (NULL);
	path_dirs = ft_split(envp[i] + 5, ':');
	if (!path_dirs)
		return (NULL);
	path = search_in_path(name, path_dirs);
	free_path_dirs(path_dirs);
	if (path == NULL)
		printf("minishell: %s: command not found\n", name);
	return (path);
}

static char	*check_dir_path(char *cmd_name)
{
	if (ft_strchr(cmd_name, '/') != NULL)
	{
		if (access(cmd_name, X_OK) == 0)
			return (ft_strdup(cmd_name));
		perror(cmd_name);
		return (NULL);
	}
	return (NULL);
}

static char	*search_in_path(char *cmd_name, char **path_dirs)
{
	int		i;
	char	*full_path;
	char	*tmp_path;

	i = 0;
	while (path_dirs[i] != NULL)
	{
		tmp_path = ft_strjoin(path_dirs[i], "/");
		full_path = ft_strjoin(tmp_path, cmd_name);
		free(tmp_path);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}
