#include "../../includes/minishell.h"

bool	init_heredoc(char *limiter, t_shell *shell)
{
	char	*line;
	int		pipe_nbr;
	pid_t	id;
	int		status;

	id = fork();
	if (id == 0)
	{
		shell->here_doc = true;
		signals(HERE);
		while (true)
		{
			pipe_nbr = shell->pipe_nbr;
			while (pipe_nbr-- > 0)
				ft_putstr_fd("pipe ", 1);
			line = readline("heredoc> ");
			if (!line)
				break ;
			if (line[ft_strlen(line) - 1] == '\n')
				line[ft_strlen(line) - 1] = '\0';
			if (ft_strcmp(line, limiter) == 0)
				break ;
			ft_putendl_fd(line, shell->fds_heredoc[WRITE_END]);
			free(line);
		}
		close(shell->fds_heredoc[WRITE_END]);
		free(line);
	}
	signals(MAIN);
	close(shell->fds_heredoc[WRITE_END]);
	waitpid(id, &status, 0);
	shell->status = WEXITSTATUS(status);
	if (shell->status == 130)
		return (false);
	return (true);
}

bool	open_files(t_shell *shell, t_redir_list *file)
{
	t_redir_list	*current;
	char			*error_file;

	current = file;
	error_file = NULL;
	while (current != NULL)
	{
		if (current->type == RE_INPUT || current->type == HEREDOC)
		{
			if (current->type == RE_INPUT)
				shell->infile = open(current->file, O_RDONLY);
			else if (current->type == HEREDOC)
			{
				if (pipe(shell->fds_heredoc) == -1)
					return (false);
				if (!init_heredoc(current->file, shell))
					return (false);
				shell->infile = shell->fds_heredoc[READ_END];
			}
			if (shell->infile < 0)
			{
				shell->status = 1;
				error_file = file->file;
			}
		}
		else if (current->type == RE_OUTPUT || current->type == APPEND)
		{
			if (current->type == RE_OUTPUT)
				shell->outfile = open(current->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			else if (current->type == APPEND)
				shell->outfile = open(current->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
			if (shell->outfile < 0)
				return (false);
		}
		current = current->next;
	}
	if (error_file)
		return (error_message("zsh: no such file or directory: ", error_file), false);
	return (true);
}
