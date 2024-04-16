#include "../includes/minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	t_shell	shell;

	(void)argv;
	if (argc != 1)
		exit_error_message("Invalid number of arguments", 1);
	signals(MAIN_PROMPT);
	fprintf(stderr, "\n"); // temporary (for easy to undestand tests)
	init_shell(&shell, envp);
	exit_info(&shell);
	loop(&shell);
	clean_up(&shell, true);
	fprintf(stderr, "\n"); // temporary (for easy to undestand tests)
	return (0);
}
