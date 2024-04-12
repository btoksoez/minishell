#include "../../includes/minishell.h"

void	loop(t_shell *shell)
{
	while (true)
	{
		get_prompt(shell);
		signals(MAIN);
		if (!shell->line)
			break ;
		if (*shell->line)
			add_history(shell->line);
		shell->tokens = tokenize(shell);
		if (check_tokens(shell->tokens))
		{
			shell->status = 2;
			continue ;
		}
		expand(shell);
		if (!shell->tokens)
			continue;
		shell->tree = parse_commandline(shell->tokens, shell);
		execute(shell);
		close_all_fds(shell, false);
		wait_pids(shell->pipe_nbr + 1, shell);
		reset(shell);
	}
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

void	wait_pids(int fds, t_shell *shell)
{
	t_tree_node	*current;
	int			status;
	int			i;

	i = 0;
	current = shell->tree;
	while (current && current->right)
	{
		current = current->right;
		if (current->builtin != NULL)
			return ;
	}
	while (i < (fds - 1) - shell->builtins - 1)
		waitpid(shell->id[i++], NULL, 0);
	current = shell->tree;
	while (current && current->right)
		current = current->right;
	if (current->builtin != NULL)
		return ;
	shell->status = 0;
	waitpid(shell->id[shell->pipe_nbr], &status, 0);
	//handle status -> print newline..
	shell->status = WEXITSTATUS(status);
}

void	reset(t_shell *shell)
{
	free_all(shell);
	shell->pipe_nbr = 0;
	shell->builtins = 0;
	shell->tokens = NULL;
	shell->line = NULL;
	shell->here_doc = 0;
	shell->builtins = 0;
	shell->fd = NULL;
	shell->id = NULL;
	shell->tree = NULL;
	shell->fds_heredoc[0] = 0;
	shell->fds_heredoc[1] = 0;
	reset_fds(shell);
}

void	reset_fds(t_shell *shell)
{
	shell->infile = 0;
	shell->outfile = 0;
	if (dup2(shell->std_fds[0], STDIN_FILENO) == -1)
		error_message("Failed to reset stdin", NULL);
	if (dup2(shell->std_fds[1], STDOUT_FILENO) == -1)
		error_message("Failed to reset stdout", NULL);
	if (shell->fds_heredoc[READ_END])
		close(shell->fds_heredoc[READ_END]);
}
