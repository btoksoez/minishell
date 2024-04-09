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
	int	exit_flag;

	sign = 1;
	exit_flag = 1;
	if (shell->pipe_nbr > 0)
		exit_flag = 0;
	if (!tree->args)
	{
		shell->status = 0;
		if (exit_flag)
			clean_up(shell, 1);
		return (EXIT_SUCCESS);
	}
	if (!ft_strcmp("+", tree->args->arg) || !ft_strcmp("-", tree->args->arg))
	{
		if (*(tree->args->arg) == '-')
			sign = -1;
		tree->args = tree->args->next;
	}
	if (!is_digit_string(tree->args->arg))
	{
		ft_putstr_fd("exit\nminishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(tree->args->arg, STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		shell->status = 255;
		if (exit_flag)
			clean_up(shell, 0);
	}
	if (tree->args->next)
	{
		shell->status = 1;
		return (error_message("exit\nminishell: exit: too many arguments"), EXIT_FAILURE);
	}
	shell->status = (sign * ft_atoi(tree->args->arg)) % 256;
	if (exit_flag)
		clean_up(shell, 1);
	return (shell->status);
}
