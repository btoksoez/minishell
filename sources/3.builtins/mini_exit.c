#include "../../includes/minishell.h"

int	is_digit_string(char *str)
{
	if (!str)
		return (0);
	if (ft_atoi(str) == 0)
		return (0);
	return (1);
}
/* returns 0 if no exit, 1 if error */
int	mini_exit(t_shell *shell, t_tree_node *tree)
{
	int	sign;

	sign = 1;
	(void)tree;
	if (!tree->args)
	{
		shell->status = 0;
		clean_up(shell, 1);
	}
	if (!ft_strcmp("+", tree->args->arg) || !ft_strcmp("-", tree->args->arg))
	{
		if (*(tree->args->arg) == '-')
			sign = -1;
		tree->args = tree->args->next;
	}
	if (tree->args->next)
	{
		shell->status = 1;
		return (error_message("exit\nminishell: exit: too many arguments"), EXIT_FAILURE);
	}
	if (!is_digit_string(tree->args->arg))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(tree->args->arg, STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		shell->status = 255;
		clean_up(shell, 1);
	}
	shell->status = sign * ft_atoi(tree->args->arg) % 256;
	clean_up(shell, 1);
	return (0);
}
