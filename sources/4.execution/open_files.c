#include "../../includes/minishell.h"

bool	init_heredoc(char *limiter, t_shell *shell, int flag_in, int flag_out)
{
	char	*line;
	int		pipe_nbr;
	pid_t	id;
	int		status;

	shell->here_doc = true;
	id = fork();
	if (id == 0)
	{
		signals(HERE);
		close(shell->fds_heredoc[READ_END]);
		while (true)
		{
			pipe_nbr = shell->pipe_nbr;
			while (pipe_nbr-- > 0)
				ft_putstr_fd("pipe ", 1);
			line = readline("heredoc> ");
			if (!line)
			{
				ft_putendl_fd("minishell: warning: here-document delimited by end-of-file", STDERR_FILENO);
				break ;
			}
			if (line[ft_strlen(line) - 1] == '\n')
				line[ft_strlen(line) - 1] = '\0';
			if (ft_strcmp(line, limiter) == 0)
				break ;
			ft_putendl_fd(line, shell->fds_heredoc[WRITE_END]);
			free(line);
		}
		close(shell->fds_heredoc[WRITE_END]);
		clean_up(shell, false);
		exit (0);
	}
	signals(MAIN);
	close(shell->fds_heredoc[WRITE_END]);
	waitpid(id, &status, 0);
	if (WEXITSTATUS(status) == 130)
	{
		shell->status = 130;
		return (false);
	}
	if (flag_in != 1 && flag_out != 1)
		shell->status = status;
	return (true);
}

void	reset_heredoc_fds(t_shell *shell)
{
	if (shell->fds_heredoc[READ_END])
		close(shell->fds_heredoc[READ_END]);
	if (shell->fds_heredoc[WRITE_END])
		close(shell->fds_heredoc[WRITE_END]);
	shell->fds_heredoc[WRITE_END] = 0;
	shell->fds_heredoc[READ_END] = 0;
}

bool	open_files(t_shell *shell, t_redir_list *file)
{
	t_redir_list	*current;
	int				flag_out;
	int				flag_in;

	current = file;
	flag_out = 0;
	flag_in = 0;
	while (current != NULL)
	{
		if (current->type == RE_INPUT || current->type == HEREDOC)
		{
			if (shell->infile > 0)
			{
				close(shell->infile);
				shell->infile = 0;
			}
			if (current->type == RE_INPUT)
				shell->infile = open(current->file, O_RDONLY);
			else if (current->type == HEREDOC)
			{
				reset_heredoc_fds(shell);
				if (pipe(shell->fds_heredoc) == -1)
					return (false);
				if (!init_heredoc(current->file, shell, flag_in, flag_out))
					return (false);
				shell->infile = shell->fds_heredoc[READ_END];
			}
			if (shell->infile < 0)
			{
				shell->status = 1;
				if (flag_in == 0 && flag_out == 0)
					shell->error_file = current->file;
				flag_in = 1;
			}
		}
		else if (current->type == RE_OUTPUT || current->type == APPEND)
		{
			if (shell->outfile > 0)
			{
				close(shell->outfile);
				shell->outfile = 0;
			}
			if (current->type == RE_OUTPUT)
				shell->outfile = open(current->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			else if (current->type == APPEND)
				shell->outfile = open(current->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
			if (shell->outfile < 0)
			{
				shell->status = 1;
				if (flag_in == 0 && flag_out == 0)
					shell->error_file = current->file;
				flag_out = 1;
			}
		}
		current = current->next;
	}
	if (shell->error_file)
	{
		if (flag_in && access(shell->error_file, R_OK) != 0)
		{
			if (errno == EACCES)
				return (error_message("minishell: permission denied: ", shell->error_file), false);
			else if (errno == ENOENT)
				return (error_message("minishell: no such file or directory: ", shell->error_file), false);
			else if (errno == ENOTDIR)
				return (error_message("minishell: not a directory: ", shell->error_file), false);
		}
		if (flag_out && access(shell->error_file, R_OK) != 0)
		{
			if (errno == EACCES)
				return (error_message("minishell: permission denied: ", shell->error_file), false);
			else if (errno == ENOENT)
				return (error_message("minishell: no such file or directory: ", shell->error_file), false);
			else if (errno == ENOTDIR)
				return (error_message("minishell: not a directory: ", shell->error_file), false);
		}
	}
	return (true);
}
