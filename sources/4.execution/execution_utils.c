#include "../../includes/minishell.h"

char	**get_full_cmd(t_tree_node *node)
{
	t_args	*current;
	char	**command;
	int		arg_nbr;
	int		i;

	current = node->args;
	arg_nbr = 0;
	while (current != NULL)
	{
		current = current->next;
		arg_nbr++;
	}
	command = (char **)malloc(sizeof(char *) * (arg_nbr + 2));
	if (!command)
		error_message("Memory allocation Failed", NULL);
	command[0] = ft_strdup(node->cmd);
	i = 1;
	current = node->args;
	while (current != NULL)
	{
		command[i++] = ft_strdup(current->arg);
		current = current->next;
	}
	command[i] = NULL;
	return (command);
}

void	redirect_input_output(t_shell *shell, int i, bool last_cmd)
{
	if (shell->infile)
	{
		if (dup2(shell->infile, STDIN_FILENO) == -1)
			error_message("Error setting infile to STDIN", NULL);
	}
	else if (i != 0)
	{
		if (dup2(shell->fd[i][READ_END], STDIN_FILENO) == -1)
			error_message("Error setting pipe read end to STDIN", NULL);
	}
	if (shell->outfile)
	{
		if (dup2(shell->outfile, STDOUT_FILENO) == -1)
			error_message("Error setting outfile to STDOUT", NULL);	//change message and error code
	}
	else if (!last_cmd)
	{
		if (dup2(shell->fd[i + 1][WRITE_END], STDOUT_FILENO) == -1)
			error_message("Error setting pipe write end to STDOUT", NULL);
	}
}
