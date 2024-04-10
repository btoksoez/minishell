#include "../../includes/minishell.h"

void	init_heredoc(char *limiter, t_shell *shell)
{
	char	*line;
	int		pipe_nbr;
	int		fd;

	fd = open("here_doc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		error_message("Failed to create here_doc");
	shell->here_doc = true;
	while (true)
	{
		pipe_nbr = shell->pipe_nbr;
		while (pipe_nbr-- > 0)
			ft_putstr_fd("pipe ", 1);
		ft_putstr_fd("heredoc> ", 1);
		line = get_next_line(INPUT);
		if (!line)
			break ;
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if (ft_strcmp(line, limiter) == 0)
			break ;
		ft_putendl_fd(line, fd);
		free(line);
	}
	free(line);
	if (fd >= 0)
		close(fd);
}

void	open_files(t_shell *shell, t_redir_list *file)
{
	t_redir_list *current = file;

	while (current != NULL)
	{
		if (current->type == RE_INPUT || current->type == HEREDOC)
		{
			if (current->type == RE_INPUT)
				shell->infile = open(current->file, O_RDONLY);
			else if (current->type == HEREDOC)
			{
				init_heredoc(current->file, shell);
				shell->infile = open("here_doc", O_RDONLY);
			}
			if (shell->infile < 0)
				child_error_message(shell, "zsh: no such file or directory: ", file->file, 127);
		}
		else if (current->type == RE_OUTPUT || current->type == APPEND)
		{
			if (current->type == RE_OUTPUT)
				shell->outfile = open(current->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			else if (current->type == APPEND)
				shell->outfile = open(current->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
			if (shell->outfile < 0)
				error_message("Failed to open outfile");
		}
		current = current->next;
	}
}