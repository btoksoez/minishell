#include "../includes/minishell.h"

void	copy_envp(t_shell *shell, char **envp)
{
	int	len;
	int	i;

	len = 0;
	while (envp[len])
		len++;
	shell->envp = (char **)malloc(sizeof(char *) * (len + 1));
	if (!shell->envp)
		exit_error_message("Envp memory allocation failed", 1);
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

void	init_missing_env(t_shell *shell)
{
	char	path[1024];

	shell->envp = (char **)malloc(sizeof(char *) * 8);
	if (!shell->envp)
		exit_error_message("Envp memory allocation failed", 1);
	if (getcwd(path, sizeof(path)) != NULL)
	{
		shell->envp[0] = ft_strjoin("HOME=", path);
		shell->envp[1] = ft_strjoin("PWD=", path);
		shell->envp[2] = ft_strjoin("OLDPWD=", path);
		shell->envp[3] = ft_strdup("SHLVL=1");
		shell->envp[4] = ft_strdup("_=/usr/bin/env");
		shell->envp[5] = ft_strdup("TERM=xterm-256color");
		shell->envp[6] = NULL;
	}
}

//add extract_user() function

void	init_shell(t_shell *shell, char **envp)
{
	if (!*envp || !envp)
		init_missing_env(shell);
	else
	{
		copy_envp(shell, envp);
		shell->env_status = TRUE;
	}
	shell->infile = 0;
	shell->outfile = 0;
	shell->pipe_nbr = 0;
	shell->status= 0;
	shell->builtin_status = 0;
	shell->line = NULL;
	shell->tree = NULL;
	shell->tokens = NULL;
	shell->std_fds[0] = dup(STDIN_FILENO);
	shell->std_fds[1] = dup(STDOUT_FILENO);
	shell->envps = init_envps();
}
