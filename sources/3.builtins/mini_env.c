#include "../../includes/minishell.h"

int	mini_env(t_shell *shell, t_tree_node *tree)
{
	int	i;

	i = 0;
	while (shell->envp[i])
	{
		ft_putendl_fd(shell->envp[i], STDOUT_FILENO);
		i++;
	}
	(void)tree;
	return (EXIT_SUCCESS);
}
