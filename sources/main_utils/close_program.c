#include "../../includes/minishell.h"

void	clean_up(t_shell *shell)
{
	close(shell->std_fds[0]);
	close(shell->std_fds[1]);
	free_all(shell);
	// close_all_fds(shell);
	exit_error_message("exit", shell->status);
}

void	error_message(char *message)
{
	ft_putendl_fd(message, STDERR_FILENO);
}

void	exit_error_message(char *message, int exit_code)
{
	ft_putendl_fd(message, STDERR_FILENO);
	exit (exit_code);
}

void	close_all_fds(t_shell *shell)
{
	int	childs;
	int	i;

	i = 0;
	childs = shell->pipe_nbr + 1;
	if (shell->infile > 0)
		close(shell->infile);
	if (shell->outfile > 0)
		close(shell->outfile);
	while (i < childs)
	{
		if (shell->fd[i][WRITE_END] > 0)
			close(shell->fd[i][WRITE_END]);
		if (shell->fd[i][READ_END] > 0)
			close(shell->fd[i][READ_END]);
		i++;
	}
	if (shell->here_doc)
		unlink("here_doc");
}

void	child_error_message(t_shell *shell, char *str, char *cmd, int code)
{
	ft_putstr_fd(str, 2);
	if (cmd)
	{
		ft_putendl_fd(cmd, 2);
		free(cmd);
	}
	else
		ft_putchar_fd('\n', 2);
	close_all_fds(shell);
	exit (code);
}
