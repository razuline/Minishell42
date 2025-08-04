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
		{
			fprintf(stderr, "Unknown redirection type: %d\n", redir->type);
		}
		redir = redir->next;
	}
	return (0);
}
