#include "../includes/minishell.h"

void	clean_up(t_shell *shell)
{
	if (shell->line)
		free(shell->line);
	printf("exit\n");	//just for test purposes
	// free_tokens(shell->tokens);
}

void	error_message(char *message)
{
	ft_putendl_fd(message, STDERR_FILENO);
}

void	exit_error_message(char *message)
{
	ft_putendl_fd(message, STDERR_FILENO);
	exit (1);
}

void	close_all_fds(t_shell *shell)
{
	int	pipes;
	int	i;

	// printf("\nOpen file descriptors before reset:\n");
	// for (int fd = 0; fd <= 1000; fd++)
	// {
	// 	int flags = fcntl(fd, F_GETFL);
	// 	if (flags != -1)
	// 		printf("File descriptor %d is open\n", fd);
	// }
	// printf("\n");
	i = 0;
	fprintf(stderr, "AFTER PIPES: %d\n", shell->pipe_nbr);
	pipes = shell->pipe_nbr;
	if (shell->infile >= 0)
		close(shell->infile);
	if (shell->outfile >= 0)
		close(shell->outfile);
	while (i < pipes)
	{
		if (shell->fd[i][WRITE_END] >= 0)
			close(shell->fd[i][WRITE_END]);
		if (shell->fd[i][READ_END] >= 0)
			close(shell->fd[i][READ_END]);
		i++;
	}
	if (shell->here_doc)
		unlink("here_doc");
	// close(STDIN_FILENO);
	// close(STDOUT_FILENO);
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
	// free_all(shell);
	exit (code);
}