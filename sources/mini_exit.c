#include "../includes/minishell.h"

int	mini_exit(t_shell *shell, t_tree_node *tree)
{
	(void)tree;
	clean_up(shell);
	return (EXIT_SUCCESS);
}
