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

void	free_and_close(int fd, char **paths, char *path, char *path_cmd)
{
	if (!path_cmd)
	{
		if (path)
			free(path);
		if (fd >= 0)
			close(fd);
		ft_freematrix(paths);
	}
	else
	{
		free(path);
		free(path_cmd);
	}
}

void	get_path_index(char **envp, int *index)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			*index = i;
			return ;
		}
		i++;
	}
}

char	*get_path(char *cmd, char **envp)
{
	char	**paths;
	char	*single_path;
	char	*path_cmd;
	int		fd;
	int		i;

	get_path_index(envp, &i);
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		single_path = ft_strjoin(paths[i], "/");
		path_cmd = ft_strjoin(single_path, cmd);
		fd = open(path_cmd, O_RDONLY);
		if (fd >= 0)
		{
			free_and_close(fd, paths, single_path, NULL);
			return (path_cmd);
		}
		free_and_close(fd, paths, single_path, path_cmd);
		i++;
	}
	free_and_close(fd, paths, NULL, NULL);
	return (NULL);
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

void	execute(t_shell *shell)
{
	if (shell->tree->type == PIPE_TREE)
		execute_pipe(shell, shell->tree->left, shell->tree->right);
	else
	{
		shell->id[0] = fork();
		if (shell->id[0] == -1)
			error_message(" Failed to execute fork");
		if (shell->id[0] == 0)
			execute_command(shell, shell->tree);
	}
}
