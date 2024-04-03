#include "../includes/minishell.h"

void	reset(t_shell *shell)
{
	shell->pipe_nbr = 0;
	shell->infile = 0;
	shell->outfile = 0;

	if (dup2(shell->std_fds[0], STDIN_FILENO) == -1)
		error_message("Failed to reset stdin");
	if (dup2(shell->std_fds[1], STDOUT_FILENO) == -1)
		error_message("Failed to reset stdout");
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
	t_tree_node	*current;
	int	status;
	int	i;

	i = 0;
	shell->status = 0;
	while (i < fds - 1)
		waitpid(shell->id[i++], NULL, 0);
	current = shell->tree;
	while (current)
	{
		if (shell->tree->builtin != NULL)
			return ;
		current = current->right;
	}
	waitpid(shell->id[i], &status, 0);
	shell->status = WEXITSTATUS(status);
}

void	get_prompt(t_shell *shell)
{
	char	*exit_status;
	char	*prompt;

	if (shell->status)
		exit_status = ft_strjoin(RED, "⇾ ");
	else
		exit_status = ft_strjoin(GREEN, "⇾ ");
	exit_status= ft_strjoin(exit_status, RESET);

	prompt = ft_strjoin(CYAN, "minishell");
	prompt = ft_strjoin(prompt, RESET);

	shell->line = ft_strjoin(exit_status, prompt);
	shell->line = ft_strjoin(shell->line, "$ ");
	shell->line = readline(shell->line);
}

void	loop(t_shell *shell)
{
	while (true)
	{
		// if (g_sig == SIGQUIT)
		// 	break;
		shell->tokens = NULL;
		get_prompt(shell);
		if (!shell->line)
			break;
		if (*shell->line)
			add_history(shell->line);
		if (ft_strcmp(shell->line, "exit") == 0)
			break ;
		if (check_syntax_errors(shell->line))
			continue ;
		shell->tokens = tokenize(shell);
		if (check_tokens(shell->tokens))
			continue ;
		// expand(tokens);
		if (!shell->tokens)
		{
			reset(shell);
			continue;
		}
		shell->tree = parse_commandline(shell->tokens);
		// print_tokens(shell->tokens);
		// print_tree(shell->tree, 0);
		prepare_to_execute(shell);
		execute(shell);
		close_all_fds(shell);
		wait_pids(shell->pipe_nbr + 1, shell);
		free(shell->line);
		reset(shell);										//reset lists of tokens etc, but keep history
	}
}
