/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 15:28:17 by preltien          #+#    #+#             */
/*   Updated: 2025/08/09 14:13:47 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 🔹 Récupère la valeur de PATH depuis envp, ou retourne une valeur par défaut
char	*get_path_from_env(t_shell *state)
{
	int		i;
	char	*path_env;

	i = 0;
	while (state->envp[i])
	{
		if (strncmp(state->envp[i], "PATH=", 5) == 0)
			return (strdup(state->envp[i] + 5));
		i++;
	}
	path_env = strdup(
			"/home/preltien/bin:/usr/local/sbin:/usr/local/bin:"
			"/usr/sbin:/usr/bin:/sbin:/bin");
	return (path_env);
}

// 🔹 Essaie de trouver le binaire exécutable dans les chemins fournis
char	*find_executable_path(char *cmd, char **path_split)
{
	int		i;
	char	*bin;
	size_t	len;

	i = 0;
	while (path_split[i])
	{
		len = strlen(path_split[i]) + strlen(cmd) + 2;
		bin = (char *)calloc(len, sizeof(char));
		if (!bin)
			return (NULL);
		strcpy(bin, path_split[i]);
		strcat(bin, "/");
		strcat(bin, cmd);
		if (access(bin, F_OK | X_OK) == 0)
			return (bin);
		free(bin);
		i++;
	}
	return (NULL);
}

// 🔹 Fonction principale refactorisée
void	get_absolute_path(char **cmd, t_shell *state)
{
	char	*path_env;
	char	*bin;
	char	**path_split;

	if (!cmd || !cmd[0])
		return ;
	if (cmd[0][0] == '/' || strncmp(cmd[0], "./", 2) == 0)
		return ;
	path_env = get_path_from_env(state);
	if (!path_env)
		return ;
	path_split = ft_split(path_env, ':');
	free(path_env);
	if (!path_split)
		return ;
	bin = find_executable_path(cmd[0], path_split);
	ft_free_array(path_split);
	if (bin)
	{
		free(cmd[0]);
		cmd[0] = bin;
	}
}
