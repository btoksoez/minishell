#include "../../includes/minishell.h"

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
	*index = -1;
}

char	*get_path(char *cmd, char **envp)
{
	char	**paths;
	char	*single_path;
	char	*path_cmd;
	int		fd;
	int		i;

	get_path_index(envp, &i);
	if (i == -1)
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		single_path = ft_strjoin(paths[i], "/");
		path_cmd = ft_strjoin(single_path, cmd);
		fd = open(path_cmd, O_RDONLY);
		if (fd >= 0)
		{
			free_and_close_path(fd, paths, single_path, NULL);
			return (path_cmd);
		}
		free_and_close_path(fd, paths, single_path, path_cmd);
		i++;
	}
	free_and_close_path(fd, paths, NULL, NULL);
	return (NULL);
}

void	free_and_close_path(int fd, char **paths, char *path, char *path_cmd)
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

void	invalid_path(char **full_command, t_shell *shell, char *command)
{
	ft_freematrix(full_command);
	child_error_message(shell, "minishell: command not found: ", command, 127);
}