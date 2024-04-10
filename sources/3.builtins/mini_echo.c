#include "../../includes/minishell.h"

/* take all args as input and write to STDOUT (with space in between)
if first arg is -n, then don't write newline */
int	mini_echo(t_shell *shell, t_tree_node *cmd_node)
{
	int		nl_flag;
	t_args	*current;

	if (!cmd_node || !cmd_node->args)
	{
		ft_putchar_fd('\n', 1);
		return (EXIT_SUCCESS);
	}
	nl_flag = 1;
	current = cmd_node->args;
	while (ft_strncmp(current->arg, "-n", 2) == 0)	//first arg after echo is -n, set first arg to next one
	{
		nl_flag = 0;
		current =current->next;
	}
	while (current)
	{
		ft_putstr_fd(current->arg, 1);
		if (current->next && current->space)
			ft_putchar_fd(' ', 1);
		current = current->next;
	}
	if (nl_flag)
		ft_putchar_fd('\n', 1);
	(void)shell;
	shell->builtin_status = 0;
	return (EXIT_SUCCESS);
}