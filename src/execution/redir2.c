/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: preltien <preltien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:32:55 by preltien          #+#    #+#             */
/*   Updated: 2025/08/04 16:54:57 by preltien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// redir.c
#include "minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

static int	open_redir_out(const char *file, int flags, const char *msg)
{
	int	fd;

	fd = open(file, flags, 0644);
	if (fd < 0)
	{
		perror(msg);
		return (-1);
	}
	return (fd);
}

static int	redir_output(const char *file, int append)
{
	int	fd;

	if (append)
		fd = open_redir_out(file, O_WRONLY | O_CREAT | O_APPEND, "open >>");
	else
		fd = open_redir_out(file, O_WRONLY | O_CREAT | O_TRUNC, "open >");
	if (fd < 0)
		return (-1);
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2 stdout");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	redir_input(const char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		perror("open <");
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2 stdin");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	apply_redirections(t_redir *redir)
{
	while (redir)
	{
		if (redir->type == REDIRECT_OUT || redir->type == APPEND_OUT)
		{
			if (redir_output(redir->file, redir->type == APPEND_OUT) < 0)
				return (-1);
		}
		else if (redir->type == REDIRECT_IN)
		{
			if (redir_input(redir->file) < 0)
				return (-1);
		}
		else if (redir->type == HEREDOC)
		{
			if (here_document(redir->file) < 0)
			{
				fprintf(stderr, "heredoc failed\n");
				return (-1);
			}
		}
		else
			fprintf(stderr, "Unknown redirection type: %d\n", redir->type);
		redir = redir->next;
	}
	return (0);
}
