#include "../../includes/minishell.h"

/* with no args: change envp->pwd to envp->home
with a given path: extend path to make it absolute then update pwd and olpwd
if first argument is -: change to pwd to OLDPWD
always needs to change both the "real" working directory of process with chdir
and update the environemtn variables PWD and OLDPWD
*/

/* not sure if this might be helpful later */
void	change_directory(t_shell *s)
{
	char	*new_pwd;
	char	*new_oldpwd;
	char	cwd[1024];

	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("get cwd error\n");
		return ;
	}
	new_pwd = NULL;
	new_oldpwd = NULL;
	if (s->envps->pwd_index != -1)
	{
		free(s->envp[s->envps->pwd_index]);	//free old value
		new_pwd = ft_strjoin("PWD=", cwd);	//join new_cd and "PWD=" and '\0'
		s->envp[s->envps->pwd_index] = new_pwd;
	}
	if (s->envps->oldpwd_index != -1)
	{
		free(s->envp[s->envps->oldpwd_index]);	//free old value
		new_oldpwd = ft_strjoin("OLDPWD=", s->envps->pwd);	//join pwd and "OLDPWD=" and '\0'
		s->envp[s->envps->oldpwd_index] = new_oldpwd;
	}
}
/* the struct is used to store info about the path (size, permissions etc.)
stat returns 0 if the file or directory exists (=valid path) and -1 otherwise
so is_valid_path returns 1 if valid path, 0 otherwise */
int	is_valid_path(char *path)
{
	struct stat	buffer;

	return (stat(path, &buffer) == 0);
}

/* checks if path is a valid path, or tries to make it an absolute path and then checks if its valid
else returns NULL */
char	*extend_path(char *path)
{
	char	*extended_path;
	char	cwd[1024];

	if (!path)
		return (error_message("no path given"), NULL);
	extended_path = NULL;
	if (is_valid_path(path))
			return (path);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		if (ft_strnstr(path, cwd, ft_strlen(cwd)))
			extended_path = path;
		else
			extended_path = ft_strjoin(cwd, path);	//join with given path to make it absolute path
		if (is_valid_path(extended_path))
			return (extended_path);
		free(extended_path);
	}
	return (NULL);
}

/* gets values of PWD, OLDPWD and HOME from envp */
void	get_env_vars(t_shell *s)
{
	int	i;

	i = 0;
	while (s->envp[i])
	{
		if (ft_strncmp(s->envp[i], "PWD=", 4) == 0)
		{
			s->envps->pwd_index = i;
			s->envps->pwd = s->envp[i] + 4;
		}
		else if (ft_strncmp(s->envp[i], "OLDPWD=", 7) == 0)
		{
			s->envps->oldpwd_index = i;
			s->envps->oldpwd = s->envp[i] + 7;
		}
		else if (ft_strncmp(s->envp[i], "HOME=", 5) == 0)
			s->envps->home = s->envp[i] + 5;
		i++;
	}
}

int	mini_cd(t_shell *s, t_tree_node *tree)
{
	char	*path;
	char	*new_cd;

	if (!tree || !s)
		return (error_message("mini cd error"), EXIT_FAILURE);
	path = NULL;
	new_cd = NULL;
	if (tree->args)
		path = tree->args->arg;	//set path to first argument given after cd
	get_env_vars(s);
	if (!path)
		new_cd = ft_strdup(s->envps->home);	//no arguments, new_cd = home
	else if (ft_strncmp(path, "-", ft_strlen(path)) == 0)
		new_cd = ft_strdup(s->envps->oldpwd);	// "-", change PWD to OLDPWD
	else
	{
		if (extend_path(path))
			new_cd = ft_strdup(extend_path(path));
		else
			return (error_message("no such file or directory: {path}"), EXIT_FAILURE);
	}
	// print_test_cd(s);
	if (chdir(new_cd))	//changes directory of process
	{
		ft_putstr_fd("path not set", STDERR_FILENO);	//this protection shouldn't be necessary because we already check it in extend_path
		return (EXIT_FAILURE);
	}
	change_directory(s);	//modifies environment variables
	// print_test_cd(s);
	if (new_cd)
		free(new_cd);
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
