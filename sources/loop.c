#include "../includes/minishell.h"

void loop(t_shell *shell)
{
	// char	**args;
	// int		status;

	while (1)
	{
		shell->line = readline("minishell$ ");
		if (!shell->line)
			break;
		if (*shell->line)
			add_history(shell->line);
		printf("Input: %s\n", shell->line);
		if (check_errors(shell->line))
			continue ;	//will go to next iteration of while loop, not finishing this one
		// args = parse(shell->line);
		// status = execute(args);
		if (ft_strcmp(shell->line, "exit") == 0)
			break ;
		free(shell->line);
		//reset()	//reset lists of tokens etc, but keep history
	}
}
