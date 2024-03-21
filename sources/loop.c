#include "../includes/minishell.h"

void loop(t_shell *shell)
{
	t_tree_node		*tree_head;
	t_tokens	*tokens;
	// int	status;

	while (true)
	{
		shell->line = readline("minishell$ ");
		if (!shell->line)
			break;
		if (*shell->line)
			add_history(shell->line);
		if (check_errors(shell->line))
		{
			free(shell->line);
			continue ;
		}
		// tokens = get_tokens(shell->line)
		// tree_head = parse(tokens);
		// status = execute(args);
		if (ft_strcmp(shell->line, "exit") == 0)
			break ;
		free(shell->line);
		//reset()	//reset lists of tokens etc, but keep history
	}
}
