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

t_envps	*init_envps(void)
{
	t_envps	*envps;

	envps = malloc(sizeof(t_envps));
	if (!envps)
		return (NULL);
	envps->pwd = NULL;
	envps->oldpwd = NULL;
	envps->home = NULL;
	envps->pwd_index = -1;
	envps->oldpwd_index = -1;
	return (envps);
}

void	init_shell(t_shell *shell, char **envp)
{
	if (!*envp || !envp)
	{
		shell->env_status = FALSE;
		//TODO: which envps to init if no envp
	}
	else
	{
		copy_envp(shell, envp);
		shell->env_status = TRUE;
	}
	shell->infile = 0;
	shell->outfile = 0;
	shell->pipe_nbr = 0;
	shell->status= 0;
	shell->line = NULL;
	shell->tree = NULL;
	shell->std_fds[0] = dup(STDIN_FILENO);
	shell->std_fds[1] = dup(STDOUT_FILENO);
	shell->envps = init_envps();
}
