#include "../../includes/minishell.h"

int	is_digit_string(char *str)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	sign = 1;
	result = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -sign;
		if (str[i + 1] == '-' || str[i + 1] == '+')
			return (0);
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + str[i] - 48;
		i++;
	}
	while (str[i])
	{
		if (!ft_strchr(WHITESPACE, str[i]))
			return (0);
		i++;
	}
	return (1);
}
/* returns 0 if no exit, 1 if error */
int	mini_exit(t_shell *shell, t_tree_node *tree)
{
	int		sign;
	int		exit_flag;
	t_args	*current;

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
	current = tree->args;
	if (!ft_strcmp("+", current->arg) || !ft_strcmp("-", current->arg))
	{
		if (*(current->arg) == '-')
			sign = -1;
		current = current->next;
	}
	if (!is_digit_string(current->arg))
	{
		ft_putstr_fd("exit\nminishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(current->arg, STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		shell->status = 2;
		clean_up(shell, 0);
	}
	if (current->next)
	{
		shell->status = 1;
		return (error_message("exit\nminishell: exit: too many arguments", NULL), EXIT_FAILURE);
	}
	shell->status = (sign * ft_atoi(current->arg)) % 256;
	if (exit_flag)
		clean_up(shell, 1);
	return (shell->status);
}
