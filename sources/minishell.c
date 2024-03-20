#include "../includes/minishell.h"

void	clean_up(t_shell *shell)
{
	if (shell->line)
		free(shell->line);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_shell	shell;

	loop(&shell);
	clean_up(&shell);

	return 0;
}
