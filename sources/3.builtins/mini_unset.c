#include "../../includes/minishell.h"

/* accepts one argument, remove it in envp
if no args, error message and return */

void	remove_entry_from_envp(char ***envp, int entry_index)
{
	int	len;
	int	i;
	int	j;
	char	**new_envp;

	len = 0;
	i = 0;
	j = 0;
	while ((*envp)[len])
		len++;
	if (entry_index >= len || entry_index < 0)
		return (error_message("remove entry error"));
	new_envp = malloc(sizeof(char *) * len);
	while (j < len - 1)
    {
		if (j == entry_index)
			i = 1;
        new_envp[j] = (*envp)[i + j];
        j++;
    }
	new_envp[j] = NULL;
	i = 0;
	*envp = new_envp;
}

int	mini_unset(t_shell *s, t_tree_node *tree)
{
	int	i;

	while (tree->args)
	{
		i = 0;
		while (s->envp[i])
		{
			if (ft_strequ(s->envp[i], tree->args->arg))
			{
				remove_entry_from_envp(&(s->envp), i);
				continue;	//don't increase i to stay at right position
			}
			i++;
		}
		tree->args = tree->args->next;
	}
	return (EXIT_SUCCESS);
}
