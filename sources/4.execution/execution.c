#include "../../includes/minishell.h"

void	execute_command(t_shell *shell, t_tree_node *node)
{
	char	**command;
	char	*path;

	command = NULL;
	path = NULL;
	if (node->cmd)
	{
		path = get_path(node->cmd, shell->envp);
		command = get_full_cmd(node);
	}
	if (ft_strncmp(node->cmd, "./", 2) == 0)
	{
		path = node->cmd + 2;
		execve(path, command, shell->envp);
		child_error_message(shell, "minishell: no such file or directory: ", command[0], 127);
	}
	if (!path)
		invalid_path(command, shell, node->cmd);
	close_all_fds(shell, true);
	execve(path, command, shell->envp);
	child_error_message(shell, "minishell: command not found: ", command[0], 127);
}

void	start_execution(t_shell *shell, t_tree_node *node, int i, bool last_cmd)
{
	if (node->redir_list)
		open_files(shell, node->redir_list);
	redirect_input_output(shell, i, last_cmd);
	if (node->builtin != NULL)
		shell->builtin_status = node->builtin(shell, node);
	else
	{
		shell->id[i] = fork();
		if (shell->id[i] == -1)
			error_message("Failed to execute fork");
		if (shell->id[i] == 0)
			execute_command(shell, node);
	}
	reset_fds(shell);
}

void	execute_pipe(t_shell *shell, t_tree_node *l_node, t_tree_node *r_node, int i)
{
	start_execution(shell, l_node, i, false);
	if (r_node->type == PIPE_TREE)
		execute_pipe(shell, r_node->left, r_node->right, i + 1);
	else
		start_execution(shell, r_node, i + 1, true);
}

void	prepare_to_execute(t_shell *shell)
{
	int	i;

	shell->id = (pid_t *)malloc(sizeof(pid_t) * (shell->pipe_nbr + 2));
	if (!(shell->id))
		error_message("Pid Memory allocation failed");
	shell->fd = (int **)malloc(sizeof(int *) * (shell->pipe_nbr + 2));
	if (!(shell->fd))
		error_message("Fds Memory allocation failed");
	i = 0;
	while (i < shell->pipe_nbr + 1)
	{
		shell->fd[i] = (int *)malloc(sizeof(int) * 2);
		if (!(shell->fd[i]))
			error_message("Fds Memory allocation failed");
		if (pipe(shell->fd[i]) == -1)
			error_message("Failed to create the pipe");
		i++;
	}
	shell->fd[i] = NULL;
}

void	execute(t_shell *shell)
{
	prepare_to_execute(shell);
	if (shell->tree->type == PIPE_TREE)
		execute_pipe(shell, shell->tree->left, shell->tree->right, 0);
	else
		start_execution(shell, shell->tree, 0, true);
}