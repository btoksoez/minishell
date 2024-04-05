#include "../includes/minishell.h"

void	reset(t_shell *shell)
{
	shell->pipe_nbr = 0;
	shell->infile = 0;
	shell->outfile = 0;
	shell->tokens = NULL;

	if (dup2(shell->std_fds[0], STDIN_FILENO) == -1)
		error_message("Failed to reset stdin");
	if (dup2(shell->std_fds[1], STDOUT_FILENO) == -1)
		error_message("Failed to reset stdout");
	free_all(shell);
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
	int			status;
	int			i;

	i = 0;
	shell->status = 0;
	while (i < fds - 1)
		waitpid(shell->id[i++], NULL, 0);
	current = shell->tree;
	while (current && current->right)
		current = current->right;
	if (current->builtin != NULL)
	{
		shell->status = shell->builtin_status;
		return ;
	}
	waitpid(shell->id[i], &status, 0);
	shell->status = WEXITSTATUS(status);
}

void	get_prompt(t_shell *shell)
{
	char	*exit_status;
	char	*program_name;
	char	*tmp;
	char	*prompt;

	if (shell->status)
		tmp = ft_strjoin(RED, "⇾ ");
	else
		tmp = ft_strjoin(GREEN, "⇾ ");
	exit_status = ft_strjoin(tmp, RESET);
	free(tmp);

	tmp = ft_strjoin(CYAN, "minishell");
	program_name = ft_strjoin(tmp, RESET);
	free(tmp);

	tmp = ft_strjoin(exit_status, program_name);
	prompt = ft_strjoin(tmp, "$ ");
	shell->line = readline(prompt);
	free(exit_status);
	free(program_name);
	free(prompt);
	free(tmp);
}

void	loop(t_shell *shell)
{
	while (true)
	{
		get_prompt(shell);
		if (!shell->line)
			break;
		if (*shell->line)
			add_history(shell->line);
		// if (ft_strcmp(shell->line, "exit") == 0)
		// 	break ;
		if (check_syntax_errors(shell->line))
			continue ;
		shell->tokens = tokenize(shell);
		if (check_tokens(shell->tokens))
			continue ;
		// print_tokens(shell->tokens);
		expand(shell->tokens);
		if (!shell->tokens)
			continue;
		shell->tree = parse_commandline(shell->tokens);
		execute(shell);
		close_all_fds(shell);
		wait_pids(shell->pipe_nbr + 1, shell);
		reset(shell);										//reset lists of tokens etc, but keep history
	}
}
