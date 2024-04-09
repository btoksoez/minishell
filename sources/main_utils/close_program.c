#include "../../includes/minishell.h"

void	clean_up(t_shell *shell, bool print_msg)
{
	int i;

	close_all_fds(shell, true);
	free_all(shell);
	if (shell->envps)
		free(shell->envps);
	i = 0;
	while (shell->envp[i])
		free(shell->envp[i++]);
	if (shell->envp)
		free(shell->envp);
	if (print_msg)
		exit_error_message("exit", shell->status);
}

void	error_message(char *message, char *cmd)
{
	ft_putstr_fd(message, STDERR_FILENO);
	if (cmd)
		ft_putendl_fd(cmd, STDERR_FILENO);
	else
		ft_putchar_fd('\n', 2);
}

void	exit_error_message(char *message, int exit_code)
{
	ft_putendl_fd(message, STDERR_FILENO);
	exit (exit_code);
}

void	close_all_fds(t_shell *shell, bool in_out)
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
	if (in_out)
	{
		close(shell->std_fds[0]);
		close(shell->std_fds[1]);
	}
	if (shell->here_doc)
		unlink("here_doc");
}

void	child_error_message(t_shell *shell, char *str, char *cmd, int code)
{
	ft_putstr_fd(str, STDERR_FILENO);
	if (cmd)
		ft_putendl_fd(cmd, STDERR_FILENO);
	else
		ft_putchar_fd('\n', STDERR_FILENO);
	clean_up(shell, false);
	exit (code);
}
