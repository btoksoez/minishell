#include "../includes/minishell.h"

/* with no args: change envp->pwd to envp->home
with a given path: extend path to make it absolute then update pwd and olpwd
if first argument is -: change to pwd to OLDPWD
also implement ..?
*/
int	mini_cd(t_shell *shell, t_tree_node *tree)
{
	//TODO
	//get current path
		//shell->envp, somehow find PWD, OLDPWD, HOME
	//wi
	(void)shell;
	(void)tree;
	return (1);
}
