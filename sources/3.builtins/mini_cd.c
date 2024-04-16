#include "../../includes/minishell.h"

/* with no args: change envp->pwd to envp->home
with a given path: extend path to make it absolute then update pwd and olpwd
if first argument is -: change to pwd to OLDPWD
always needs to change both the "real" working directory of process with chdir
and update the environemtn variables PWD and OLDPWD
*/

void	add_path_to_env(t_shell *s)
{
	char	*new_pwd;
	int		i;

	new_pwd = NULL;
	i = 0;
	while (s->envp[i])
	{
		if (!ft_strncmp(s->envp[i], "PWD=", 4))
		{
			free(s->envp[i]);	//free old value
			new_pwd = ft_strjoin("PWD=", s->pwd);	//join new_cd and "PWD=" and '\0'
			s->envp[i] = new_pwd;
		}
		else if (!ft_strncmp(s->envp[i], "OLDPWD=", 7) && s->oldpwd)
		{
			free(s->envp[i]);	//free old value
			new_pwd = ft_strjoin("OLDPWD=", s->oldpwd);	//join new_cd and "PWD=" and '\0'
			s->envp[i] = new_pwd;
		}
		i++;
	}
}

void	change_path(t_shell *shell)
{
	char	*temp;

	temp = ft_strdup(shell->pwd);
	free(shell->oldpwd);
	shell->oldpwd = temp;
	free(shell->pwd);
	shell->pwd = getcwd(NULL, 0);
}

/* gets values of PWD, OLDPWD and HOME from envp */
void	get_env_vars(t_shell *s)
{
	int	i;

	i = 0;
	while (s->envp[i])
	{
		if (ft_strncmp(s->envp[i], "PWD=", 4) == 0)
			s->pwd = ft_substr(s->envp[i], 4, ft_strlen(s->envp[i]) - 4);
		else if (ft_strncmp(s->envp[i], "OLDPWD=", 7) == 0)
			s->oldpwd = ft_substr(s->envp[i], 7, ft_strlen(s->envp[i]) - 7);
		i++;
	}
}

char	*find_path_ret(char *str, t_shell *s)
{
	int	i;

	i = 0;
	while (s->envp[i])
	{
		if (!ft_strncmp(s->envp[i], str, ft_strlen(str)))
			return (ft_substr(s->envp[i], ft_strlen(str),
					ft_strlen(s->envp[i]) - ft_strlen(str)));
		i++;
	}
	return (NULL);
}
/* gets the curren value of env str and sets the process directory to it */
int	specific_path(t_shell *s, char *str)
{
	char	*tmp;
	int		ret;

	ret = 1;
	tmp = find_path_ret(str, s);
	if (tmp)
		ret = chdir(tmp);
	if (ret != 0)
	{
		str = ft_substr(str, 0, ft_strlen(str) - 1);
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(str, STDERR_FILENO);
		free(str);
		ft_putendl_fd(" not set", STDERR_FILENO);
	}
	if (tmp && ft_strcmp(str, "OLDPWD=") == 0)
		ft_putendl_fd(tmp, STDERR_FILENO);
	if (tmp)
		free(tmp);
	return (ret);
}

int	mini_cd(t_shell *s, t_tree_node *tree)
{
	int		ret;

	if (!tree || !s)
		return (error_message("mini cd error", NULL), EXIT_FAILURE);
	ret = 0;
	add_missing_env(s);
	if (args_len(tree->args) > 1)
		return (ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO), EXIT_FAILURE);
	if (!tree->args)
		ret = specific_path(s, "HOME=");
	else if (ft_strncmp(tree->args->arg, "-", ft_strlen(tree->args->arg)) == 0)
		ret = specific_path(s, "OLDPWD=");
	else
	{
		ret = chdir(tree->args->arg);
		if (ret != 0)
		{
			ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
			ft_putstr_fd(tree->args->arg, STDERR_FILENO);
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		}
	}
	if (ret != 0)
		return (EXIT_FAILURE);
	change_path(s);
	add_path_to_env(s);
	return (EXIT_SUCCESS);
}


/*

chdir:

Prototype: int chdir(const char *path)
Usage Explanation: Changes the current working directory to the one specified by the path argument.
Example:


int result = chdir("/path/to/directory");
if (result == 0) {
    printf("Directory changed successfully.\n");
} else {
    perror("chdir");
}
getcwd:

Prototype: char *getcwd(char *buf, size_t size)
Usage Explanation: Retrieves the current working directory and stores it in the buffer buf with a maximum size of size.
Example:


char cwd[PATH_MAX];
if (getcwd(cwd, sizeof(cwd)) != NULL) {
    printf("Current working directory: %s\n", cwd);
} else {
    perror("getcwd");
}
stat:

Prototype: int stat(const char *pathname, struct stat *statbuf)
Usage Explanation: Retrieves information about the file specified by pathname and stores it in the statbuf structure.
Example:


struct stat fileStat;
stat("/path/to/file", &fileStat);
lstat:

Prototype: int lstat(const char *pathname, struct stat *statbuf)
Usage Explanation: Similar to stat but does not follow symbolic links. Retrieves information about the link itself.
Example:


struct stat linkStat;
lstat("/path/to/symlink", &linkStat);
opendir:

Prototype: DIR *opendir(const char *dirname)
Usage Explanation: Opens a directory stream corresponding to the given directory name.
Example:


DIR *dir = opendir("/path/to/directory");
readdir:

Prototype: struct dirent *readdir(DIR *dirp)
Usage Explanation: Reads the next entry from the directory stream opened by opendir.
Example:


struct dirent *entry;
while ((entry = readdir(dir)) != NULL) {
    printf("%s\n", entry->d_name);
}
closedir:

Prototype: int closedir(DIR *dirp)
Usage Explanation: Closes the directory stream opened by opendir when you are done reading entries from it.
Example:


closedir(dir);


*/
