#include "../includes/minishell.h"

void	loop(t_shell *shell)
{
	// char	**args;
	t_token	*tokens;
	int		status;

	(void)status;
	while (true)
	{
		tokens = NULL;
		shell->line = readline("minishell$ ");
		if (!shell->line)
			break;
		if (*shell->line)
			add_history(shell->line);
		if (ft_strcmp(shell->line, "exit") == 0)
			break ;
		tokens = tokenize(shell->line);
		if (!tokens)
			status = 1; 							// search for the right status value

		// args = parse(shell->line);
		// status = execute(args);
		// reset()	//reset lists of tokens etc, but keep history
		free(shell->line);
	}
}
