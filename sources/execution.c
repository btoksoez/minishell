#include "../includes/minishell.h"

// void	open_and_redir(t_shell *shell, t_redir_list *file)
// {
// 	(void)shell;
// 	t_redir_list *current = file;

// 	while (current != NULL)
// 	{
// 		if (file->type == RE_INPUT)
// 			open();
// 		else if (file->type == RE_OUTPUT)
// 			open();
// 		else if (file->type == APPEND)
// 			open();
// 		else if (file->type == HEREDOC)
// 			init_heredoc();
// 		current = current->next;
// 	}
// }

char	*get_full_cmd(t_tree_node *node)
{
	t_args	*current;
	char	*command;

	command = ft_strdup(node->cmd);
	current = node->args;
	if (node->args)
	{
		command = ft_strjoin(command, " ");
		while (current != NULL)
		{
			command = ft_strjoin(command, current->arg);
			command = ft_strjoin(command, " ");
			current = current->next;
		}
	}
	return (command);
}

void	execute_command(t_shell *shell, t_tree_node *node)
{
	(void)shell;
	char	*command;
	char	*command_path;

	command = NULL;
	command_path = NULL;
	// if (node->redir_list)
	// 	open_and_redir(shell, node->redir_list);

	if (node->cmd)
	{
		command_path = get_path(node->cmd, shell->envp);
		command = get_full_cmd(node);
	}
	(void)command;
	(void)command_path;
}

void	execute_pipe(t_shell *shell, t_tree_node *l_node, t_tree_node *r_node)
{
	if (r_node->type == PIPE_TREE)
		execute_pipe(shell, r_node->left, r_node->right);
	else
		execute_command(shell, r_node);
	execute_command(shell, l_node);
}

// need to add tree level to start the forks with the right pdis and fds

void	execute(t_shell *shell)
{
	if (shell->tree->type == PIPE_TREE)
		execute_pipe(shell, shell->tree->left, shell->tree->right);
	else
	{
		// shell->id[0] = fork();
		// if (shell->id[0] == -1)
		// 	error_message(" Failed to execute fork");
		// if (shell->id[0] == 0)
			execute_command(shell, shell->tree);
	}
}
