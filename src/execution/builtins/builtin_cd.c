/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 15:00:00 by preltien          #+#    #+#             */
/*   Updated: 2025/08/18 10:28:32 by erazumov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Handles the 'cd' command with no arguments by changing to HOME dir. */
static int	handle_cd_to_home(t_shell *state)
{
	char	*home_dir;

	home_dir = get_env_value("HOME", state->envp);
	if (!home_dir || *home_dir == '\0')
	{
		fprintf(stderr, "minishell: cd: HOME not set\n");
		free(home_dir);
		return (1);
	}
	if (chdir(home_dir) != 0)
	{
		fprintf(stderr, "minishell: cd: %s: %s\n", home_dir, strerror(errno));
		free(home_dir);
		return (1);
	}
	free(home_dir);
	return (0);
}

/* Updates the PWD and OLDPWD environment variables after a successful cd. */
static void	update_pwd_vars(t_shell *state)
{
	char	cwd[4096];
	char	*old_pwd;

	old_pwd = get_env_value("PWD", state->envp);
	if (old_pwd)
	{
		set_env_var(state, "OLDPWD", old_pwd);
		free(old_pwd);
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		set_env_var(state, "PWD", cwd);
}

/* Built-in command: change directory.
 * Changes the current working directory of the shell. */
int	builtin_cd(char **argv, t_shell *state)
{
	int	ret_code;

	if (argv[1] && argv[2])
	{
		fprintf(stderr, "minishell: cd: too many arguments\n");
		return (1);
	}
	if (!argv[1])
		ret_code = handle_cd_to_home(state);
	else
	{
		if (chdir(argv[1]) != 0)
		{
			fprintf(stderr, "minishell: cd: %s: %s\n", argv[1],
				strerror(errno));
			ret_code = 1;
		}
		else
			ret_code = 0;
	}
	if (ret_code == 0)
		update_pwd_vars(state);
	return (ret_code);
}
