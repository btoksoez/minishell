#include "../includes/minishell.h"

void	reset(t_shell *shell)
{
	shell->pipe_nbr = 0;
	shell->infile = 0;
	shell->outfile = 0;
	close_all_fds(shell);

	printf("\nOpen file descriptors after reset:\n");
	for (int fd = 0; fd <= 1000; fd++)
	{
		int flags = fcntl(fd, F_GETFL);
		if (flags != -1)
			printf("File descriptor %d is open\n", fd);
	}
	printf("\n");
	if (dup2(shell->std_fds[0], STDIN_FILENO) == -1)
	{
		perror("Failed to reset stdin");
		exit(EXIT_FAILURE);
	}
	if (dup2(shell->std_fds[1], STDOUT_FILENO) == -1)
	{
		perror("Failed to reset stdout");
		exit(EXIT_FAILURE);
	}
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

void	wait_pids(int fds, t_shell *shell)
{
	// int	status;
	int	i;

	i = 0;
	while (fds > 0)
	{
		waitpid(shell->id[i++], NULL, 0);
		fds--;
	}
	// waitpid(shell->id[i], &status, 0);
	// return (status);						 //change to int
}

void	loop(t_shell *shell)
{
	int			status;

	(void)status;
	while (true)
	{
		if (g_sig == 3)
			break;
		shell->tokens = NULL;
		shell->status = 0;
		shell->line = readline("minishell$ ");
		if (!shell->line)
			break;
		if (*shell->line)
			add_history(shell->line);
		if (ft_strcmp(shell->line, "exit") == 0)
			break ;
		if (check_syntax_errors(shell->line))
			continue ;
		shell->tokens = tokenize(shell->line);
		if (check_tokens(shell->tokens))
			continue ;
		expand(shell->tokens);
		print_tokens(shell->tokens);
		if (!shell->tokens)
			status = 1; 								// search for the right status value
		if (!status)
			shell->tree = parse_commandline(shell->tokens);
		// // quick test of echo
		// while (shell->tree)
		// {
		// 	if (shell->tree->builtin != NULL)
		// 		shell->tree->builtin(shell, shell->tree);
		// 	shell->tree = shell->tree->left;
		// }
		print_tree(shell->tree, 0);
		// execute(shell->tree);
		// reset()	//reset lists of tokens etc, but keep history
		// free(shell->line);
	}
}
