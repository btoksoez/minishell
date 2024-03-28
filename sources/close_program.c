#include "../includes/minishell.h"

void	clean_up(t_shell *shell)
{
	if (shell->line)
		free(shell->line);
	printf("Cleaning done\n");	//just for test purposes
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
