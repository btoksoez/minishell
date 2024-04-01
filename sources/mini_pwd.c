#include "../includes/minishell.h"

int	mini_pwd(t_shell *s, t_tree_node *tree)
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
		i++;
	}
	ft_putendl_fd(s->envps->pwd, STDOUT_FILENO);
	(void)tree;
	return (EXIT_SUCCESS);
}
