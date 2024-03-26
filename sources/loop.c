#include "../includes/minishell.h"

void	loop(t_shell *shell)
{
	t_tree_node	*tree_head;
	t_tokens	*tokens;
	t_tokens	*tokens_head;
	int			status;

	(void)status;
	(void)tree_head;
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
		check_syntax_errors(shell->line);
		tokens = tokenize(shell->line);
		//check tokens for errors, f.e. are tokens after redirections always WORDS and not ENV or other REDIR
		tokens_head = tokens;
		print_tokens(tokens_head);
		if (!tokens)
			status = 1; 								// search for the right status value
		if (!status)
			tree_head = parse_commandline(tokens);
		print_tree(tree_head, 0);
		// execute(tree_head);
		// reset()	//reset lists of tokens etc, but keep history
		free(shell->line);
	}
}
