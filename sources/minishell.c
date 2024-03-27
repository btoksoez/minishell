#include "../includes/minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	t_shell	shell;

	(void)argv;
	(void)envp;
	if (argc != 1)
		exit_error_message("Invalid number of arguments");
	signals();
	fprintf(stderr, "\n"); // temporary (for easy to undestand tests)
	loop(&shell);
	clean_up(&shell);
	fprintf(stderr, "\n"); // temporary (for easy to undestand tests)
	return (0);
}
