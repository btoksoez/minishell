#include "../includes/minishell.h"

/* with no args: change envp->pwd to envp->home
with a given path: extend path to make it absolute then update pwd and olpwd
if first argument is -: change to pwd to OLDPWD
also implement ..?
*/

/* not sure if this might be helpful later */
void	change_directory(t_shell *s, char *new_cd)
{
	char *new_pwd;

	new_pwd = NULL;
	if (s->envps->pwd_index != -1)
	{
		free(s->envp[s->envps->pwd_index]);	//free old value
		new_pwd = ft_strjoin("PWD=", new_cd);	//join new_cd and "PWD=" and '\0'
		s->envp[s->envps->pwd_index] = new_pwd;
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
	if (is_valid_path(path))	//if it's a valid path already, just return it
		return (path);
	else
	{
		if (getcwd(cwd, sizeof(cwd)) != NULL)
		{
			extended_path = ft_strjoin(cwd, path);
			if (is_valid_path(extended_path))
				return (extended_path);
			free(extended_path);
		}
	}
	return (NULL);
}

int	mini_cd(t_shell *s, t_tree_node *tree)
{
	// char	*new_oldpwd;
	char	*path;
	char	*new_cd;
	int		i;

	if (!tree || !s)
		return (error_message("mini cd error"), 0);
	i = 0;
	path = tree->args->arg;	//set path to first argument given after cd
	// go through environment variables
	while (s->envp[i])
	{
		if (ft_strncmp(s->envp[i], "OLDPWD=", 7) == 0)
		{
			s->envps->oldpwd_index = i;
			s->envps->oldpwd = s->envp[i] + 7;
		}
		else if (ft_strncmp(s->envp[i], "HOME=", 5) == 0)
			s->envps->home = s->envp[i] + 5;
		i++;
	}
	printf("pwd: %s\nold: %s\nhome: %s\n", s->envps->pwd, s->envps->oldpwd, s->envps->home);
	if (!path)
		new_cd = s->envps->home;	//no arguments, new_cd = home
	else if (ft_strncmp(path, "-", ft_strlen(path)) == 0)
		new_cd = s->envps->oldpwd;	// "-", change PWD to OLDPWD
	else
		new_cd = extend_path(path);	//get full path
	if (chdir(new_cd) == 0)
		printf("Changed directory to: %s\n", new_cd);
	else
		perror("chdir: no such file or directory");	//need to change error message
	printf("\nAfter:\n");
	print_envp(s);	//not sure if it actually changes becasue here it's still like old ones; how to check?
	//get current path
		//shell->envp, somehow find PWD, OLDPWD, HOME
	(void)tree;
	// if (new_cd)
	// 	free(new_cd);
	return (1);
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
