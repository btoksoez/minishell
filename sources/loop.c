#include "../includes/minishell.h"

void	loop(t_shell *shell)
{
	int			status;

	(void)status;
	while (true)
	{
		if (g_sig == 3)
			break;
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
		shell->tokens = tokenize(shell->line);
		if (check_tokens(shell->tokens))
			continue ;
		expand(shell->tokens);
		print_tokens(shell->tokens);
		if (!shell->tokens)
			status = 1; 								// search for the right status value
		if (!status)
			shell->tree = parse_commandline(shell->tokens);
		// // quick test of echo
		// while (shell->tree)
		// {
		// 	if (shell->tree->builtin != NULL)
		// 		shell->tree->builtin(shell, shell->tree);
		// 	shell->tree = shell->tree->left;
		// }
		print_tree(shell->tree, 0);
		// execute(shell->tree);
		// reset()	//reset lists of tokens etc, but keep history
		// free(shell->line);
	}
}
