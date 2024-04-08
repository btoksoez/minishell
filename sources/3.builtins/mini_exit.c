#include "../../includes/minishell.h"

int	is_digit_string(char *str)
{
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}
/* returns 0 if no exit, 1 if error */
int	mini_exit(t_shell *shell, t_tree_node *tree)
{
	(void)tree;
	if (!tree->args)
	{
		shell->status = 0;
		clean_up(shell, true);
	}
	if (tree->args->next)
	{
		shell->status = 1;
		return (error_message("minishell: exit: too many arguments"), EXIT_FAILURE);
	}
	if (!is_digit_string(tree->args->arg))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(tree->args->arg, STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		shell->status = 2;
		clean_up(shell, true);
	}
	shell->status = ft_atoi(tree->args->arg) % 256;
	clean_up(shell, true);
	return (0);
}
