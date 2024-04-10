#include "../../includes/minishell.h"

int	mini_pwd(t_shell *s, t_tree_node *tree)
{
	int	i;

	i = 0;
	while (s->envp[i])
	{
		if (ft_strncmp(s->envp[i], "PWD=", 4) == 0)
			s->pwd = ft_substr(s->envp[i], 4, ft_strlen(s->envp[i]) - 4);
		i++;
	}
	ft_putendl_fd(s->pwd, STDOUT_FILENO);
	(void)tree;
	return (EXIT_SUCCESS);
}
