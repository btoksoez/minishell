#include "../../includes/minishell.h"

void	loop(t_shell *shell)
{
	while (true)
	{
		signals(MAIN_PROMPT);
		get_prompt(shell);
		signals(MAIN);
		if (!shell->line)
			break ;
		if (*shell->line)
			add_history(shell->line);
		shell->tokens = tokenize(shell);
		shell->reseted = false;
		if (check_tokens(shell->tokens))
		{
			shell->status = 2;
			free_tokens(shell->tokens);
			shell->tokens = NULL;
			continue ;
		}
		expand(shell);
		remove_spaces(&shell->tokens);
		remove_empty_tokens(&shell->tokens);
		if (!shell->tokens)
			continue;
		shell->tree = parse_commandline(shell->tokens, shell);
		execute(shell);
		close_all_fds(shell, false);
		wait_pids(shell);
		reset(shell);
	}
}

void	get_prompt(t_shell *shell)
{
	char	*exit_status;
	char	*program_name;
	char	*tmp;
	char	*prompt;

	if (shell->line)
	{
		free(shell->line);
		shell->line = NULL;
	}
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

//here doc works
//cmd and invalid cmd works
//cmd with pipes works
//last cmd builtin works
//<<eof > no_permission works
//cat < valid_file | ls > out
//cat < valid_file | ls > no_permissions
//cat -> ctrld
//cat -> ctrlc
//cat -> ctrl\


//doesn't work:
//cat < invalid_file | ls > out
//cat < no_permission_file | ls > out
//echo $USER | echo $USER | cat/ls... -> if 2 or more pipes and child last


void	wait_pids(t_shell *shell)
{
	t_tree_node	*current;
	int			status;
	int			i;

	i = 0;
	status = 0;
	while (i < shell->pipe_nbr)
	{
		if (shell->id_exec[i])
			waitpid(shell->id[i], NULL, 0);
		i++;
	}
	current = shell->tree;
	while (current && current->right)
		current = current->right;
	if (current->builtin != NULL)
		return ;
	if (shell->here_doc)
	{
		waitpid(shell->id[shell->pipe_nbr], &status, 0);
		if (WEXITSTATUS(status))
			shell->status = WEXITSTATUS(status);
		return ;
	}
	if (!shell->error_file)
	{
		shell->status = 0;
		waitpid(shell->id[shell->pipe_nbr], &status, 0);
		if (WIFSIGNALED(status))
		{
			shell->status = WTERMSIG(status) + 128;
			if (shell->status == 131)
				ft_putstr_fd("Quit", STDERR_FILENO);
		}
		else
			shell->status = WEXITSTATUS(status);
	}
}

void	reset(t_shell *shell)
{
	free_all(shell);
	reset_fds(shell);
	shell->pipe_nbr = 0;
	shell->builtins = 0;
	shell->tokens = NULL;
	shell->line = NULL;
	shell->here_doc = 0;
	shell->builtins = 0;
	shell->fd = NULL;
	shell->id = NULL;
	shell->tree = NULL;
	shell->error_file = NULL;
	shell->fds_heredoc[READ_END] = 0;
	shell->fds_heredoc[WRITE_END] = 0;
	shell->reseted = true;
}

void	reset_fds(t_shell *shell)
{
	if (shell->fds_heredoc[READ_END])
		close(shell->fds_heredoc[READ_END]);
	if (shell->infile >= 0)
		close(shell->infile);
	shell->infile = 0;
	if (shell->outfile >= 0)
		close(shell->outfile);
	shell->outfile = 0;
	if (dup2(shell->std_fds[0], STDIN_FILENO) == -1)
		error_message("Failed to reset stdin", NULL);
	if (dup2(shell->std_fds[1], STDOUT_FILENO) == -1)
		error_message("Failed to reset stdout", NULL);
}
