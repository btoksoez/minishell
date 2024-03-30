#include "../includes/minishell.h"

void	clean_up(t_shell *shell)
{
	if (shell->line)
		free(shell->line);
	// free_tokens(shell->tokens);
	// free_tree(shell->tree);
	exit_error_message("exit", 0);
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
