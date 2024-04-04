#include "../includes/minishell.h"

int	mini_exit(t_shell *shell, t_tree_node *tree)
{
	(void)tree;
	//maybe add
	clean_up(shell);
	shell->builtin_status = 0;
	return (EXIT_SUCCESS);
}
