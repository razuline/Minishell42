// redir.c
#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int	here_document(const char *limiter)
{
	int		pipefd[2];
	char	*line;

	if (pipe(pipefd) == -1)
		return (perror("pipe"), -1);

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			fprintf(stderr, "warning: here-document delimited by EOF (wanted `%s`)\n", limiter);
			break;
		}
		if (strcmp(line, limiter) == 0)
		{
			free(line);
			break;
		}
		write(pipefd[1], line, strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);

	if (dup2(pipefd[0], STDIN_FILENO) == -1)
		return (perror("dup2 heredoc"), close(pipefd[0]), -1);
	close(pipefd[0]);
	return (0);
}


int apply_redirections(t_redir *redir)
{
	int	fd;

	while (redir)
	{
		if (redir->type == REDIRECT_OUT)
		{
			fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
			{
				perror("[ERROR] open for >");
				return -1;
			}
			if (dup2(fd, STDOUT_FILENO) < 0)
			{
				perror("[ERROR] dup2 for >");
				close(fd);
				return -1;
			}
			close(fd);
		}
		else if (redir->type == APPEND_OUT)
		{
			fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0)
			{
				perror("[ERROR] open for >>");
				return -1;
			}
			if (dup2(fd, STDOUT_FILENO) < 0)
			{
				perror("[ERROR] dup2 for >>");
				close(fd);
				return -1;
			}
			close(fd);
		}
		else if (redir->type == REDIRECT_IN)
		{
			fd = open(redir->file, O_RDONLY);
			if (fd < 0)
			{
				perror("[ERROR] open for <");
				return -1;
			}
			if (dup2(fd, STDIN_FILENO) < 0)
			{
				perror("[ERROR] dup2 for <");
				close(fd);
				return -1;
			}
			close(fd);
		}
		else if (redir->type == HEREDOC)
		{
			if (here_document(redir->file) < 0)
			{
				fprintf(stderr, "[ERROR] here_doc failed\n");
				return -1;
			}
		}
		else
		{
			fprintf(stderr, "[WARNING] Unknown redirection type: %d\n", redir->type);
		}
		redir = redir->next;
	}
	return (0);
}
