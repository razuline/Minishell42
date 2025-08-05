/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: preltien <preltien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:42:33 by preltien          #+#    #+#             */
/*   Updated: 2025/08/04 16:42:34 by preltien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// redir.c
#include "minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

static void	print_heredoc_warning(const char *limiter)
{
	fprintf(stderr, "warning: here-document delimited by EOF (wanted `%s`)\n",
		limiter);
}

static void	write_heredoc_line(int write_fd, const char *line)
{
	write(write_fd, line, strlen(line));
	write(write_fd, "\n", 1);
}

static int	read_and_write_heredoc(int write_fd, const char *limiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			print_heredoc_warning(limiter);
			break ;
		}
		if (strcmp(line, limiter) == 0)
		{
			free(line);
			break ;
		}
		write_heredoc_line(write_fd, line);
		free(line);
	}
	return (0);
}

int	here_document(const char *limiter)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
		return (perror("pipe"), -1);
	read_and_write_heredoc(pipefd[1], limiter);
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		perror("dup2 heredoc");
		close(pipefd[0]);
		return (-1);
	}
	close(pipefd[0]);
	return (0);
}
