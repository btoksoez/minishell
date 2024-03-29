#include "../includes/minishell.h"

void	init_heredoc(char *limiter, t_shell *shell)
{
	char	*line;
	int		pipe_nbr;
	int		fd;

	fd = open("here_doc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	shell->here_doc = true;
	while (true)
	{
		pipe_nbr = shell->pipe_nbr;
		while (pipe_nbr-- > 0)
			ft_putstr_fd("pipe ", 1);
		ft_putstr_fd("heredoc> ", 1);
		line = get_next_line(INPUT);
		if (!line)
			break ;
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if (ft_strcmp(line, limiter) == 0)
			break ;
		ft_putendl_fd(line, fd);
		free(line);
	}
	free(line);
	if (fd >= 0)
		close(fd);
}

void	open_files(t_shell *shell, t_redir_list *file)
{
	t_redir_list *current = file;

	while (current != NULL)
	{
		if (current->type == RE_INPUT || current->type == HEREDOC)
		{
			if (current->type == RE_INPUT)
				shell->infile = open(current->file, O_RDONLY);
			else if (current->type == HEREDOC)
			{
				init_heredoc(current->file, shell);
				shell->infile = open("here_doc", O_RDONLY);
			}
			if (shell->infile < 0)
				error_message("Failed to open infile");

		}
		else if (current->type == RE_OUTPUT || current->type == APPEND)
		{
			if (current->type == RE_OUTPUT)
				shell->outfile = open(current->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			else if (current->type == APPEND)
				shell->outfile = open(current->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
			if (shell->outfile < 0)
				error_message("Failed to open outfile");
		}
		current = current->next;
	}
}

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
		error_message("Memory allocation Failed");
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

void	redirect_input_output(t_shell *shell, int i)
{
	if (shell->infile)
	{
		if (dup2(shell->infile, STDIN_FILENO) == -1)
			error_message("Error setting infile to STDIN");
	}
	else
	{
		if (dup2(shell->fd[i - 1][READ_END], STDIN_FILENO) == -1)
			error_message("Error setting pipe read end to STDIN");
	}
	if (shell->outfile)
	{
		if (dup2(shell->outfile, STDOUT_FILENO) == -1)
			error_message("Error setting outfile to STDOUT");
	}
	else
	{
		if (dup2(shell->fd[i][WRITE_END], STDOUT_FILENO) == -1)
			error_message("Error setting pipe write end to STDOUT");
	}
}

void	invalid_path(char **full_command, t_shell *shell, char *command)
{
	ft_freematrix(full_command);
	child_error_message(shell, "minishell: command not found: ", command, 127);
}

void	execute_command(t_shell *shell, t_tree_node *node, int i)
{
	char	**command;
	char	*path;

	(void)i;
	command = NULL;
	path = NULL;
	if (node->redir_list)
		open_files(shell, node->redir_list);
	// redirect_input_output(shell, i);
	// printf("do you come here\n?");
	if (node->cmd)
	{
		path = get_path(node->cmd, shell->envp);
		command = get_full_cmd(node);
	}
	// if (!command_path)
	// 	invalid_path(command, shell, node->cmd);
	// close_all_fds(shell);
	// execve(command_path, command, shell->envp);
}

void	execute_pipe(t_shell *shell, t_tree_node *l_node, t_tree_node *r_node, int i)
{
	if (r_node->type == PIPE_TREE)
		execute_pipe(shell, r_node->left, r_node->right, i + 1);
	else
	{
		// shell->id[i + 1] = fork();
		// if (shell->id[i + 1] == -1)
		// 	error_message(" Failed to execute fork");
		// if (shell->id[i + 1] == 0)
			execute_command(shell, r_node, i + 1);
	}
	// shell->id[i] = fork();
	// if (shell->id[i] == -1)
	// 	error_message(" Failed to execute fork");
	// if (shell->id[i] == 0)
		execute_command(shell, l_node, i);
}

// need to add tree level to start the forks with the right pdis and fds

void	execute(t_shell *shell)
{
	if (shell->tree->type == PIPE_TREE)
		execute_pipe(shell, shell->tree->left, shell->tree->right, 1);
	else
	{
		// shell->id[0] = fork();
		// if (shell->id[0] == -1)
		// 	error_message(" Failed to execute fork");
		// if (shell->id[0] == 0)
			execute_command(shell, shell->tree, 0);
	}
}
