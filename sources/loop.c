#include "../includes/minishell.h"

void	loop(t_shell *shell)
{
	t_tree_node	*tree;
	t_tokens	*tokens;
	int			status;

	(void)status;
	(void)tree;
	while (true)
	{
		tokens = NULL;
		status = 0;
		shell->line = readline("minishell$ ");
		if (!shell->line)
			break;
		if (*shell->line)
			add_history(shell->line);
		if (ft_strcmp(shell->line, "exit") == 0)
			break ;
		if (check_syntax_errors(shell->line))
			continue ;
		tokens = tokenize(shell->line);
		if (check_tokens(tokens))
			continue ;
		//expand(tokens);
		print_tokens(tokens);
		if (!tokens)
			status = 1; 								// search for the right status value
		if (!status)
			tree = parse_commandline(tokens);
		print_tree(tree, 0);
		// execute(tree_head);
		// reset()	//reset lists of tokens etc, but keep history
		free(shell->line);
	}
}
