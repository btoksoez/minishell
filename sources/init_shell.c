#include "../includes/minishell.h"

void	copy_envp(t_shell *shell, char **envp)
{
	int	len = 0;
	int	i;

	while (envp[len])
		len++;
	shell->envp = (char **)malloc(sizeof(char *) * len + 1);
	if (!shell->envp)
		exit_error_message("Envp memory allocation failed");
	i = 0;
	while (i < len)
	{
		shell->envp[i] = ft_strdup(envp[i]);
		i++;
	}
	shell->envp[i] = NULL;
}

void	init_shell(t_shell *shell, char **envp)
{
	if (!*envp || !envp)
	{
		shell->env_status = FALSE;
	}
	else
	{
		copy_envp(shell, envp);
		shell->env_status = TRUE;
	}
	shell->infile = 0;
	shell->outfile = 0;
	shell->pipe_nbr = 0;
	shell->line = NULL;
	shell->tree = NULL;
}
