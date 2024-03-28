#include "../includes/minishell.h"

/* go through array of builtins and return function pointer if exists, else NULL */
int		(*builtin_arr(char *str))(t_shell *shell, struct t_tree_node *tree)
{
	static void	*builtins[7][2] =
	{
		{"echo", mini_echo},
		{"cd", mini_cd},
		{"pwd", mini_pwd},
		{"export", mini_export},
		{"unset", mini_unset},
		{"env", mini_env},
		{"exit", mini_exit}
	};
	int			i;

	if (!str)
		return (NULL);
	i = 0;
	while (i < 7)
	{
		if (!ft_strncmp(builtins[i][0], str, ft_strlen(builtins[i][0])))
		{
			printf("Builtin returns %s\n", (char *)builtins[i][0]);
			return (builtins[i][1]);	//returns builtin pointer if str is equal to the builtcmd
		}
		i++;
	}
	printf("Builtin returns NULL\n");
	return (NULL);
}

