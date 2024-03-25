#include "../includes/minishell.h"

/* expands environment variables in the token list,
replacing them by their value or by a empty string (\0\0) */
void	expand(t_tokens *tokens)
{
	t_tokens	*current;
	char		*env_value;

	current = tokens;
	while (current)
	{
		if (current->type == 6)
		{
			env_value = getenv(current->value);
			printf("ENV: %s\n", env_value);
			if (!env_value)
				current->value = "";
			else
				current->value = env_value;
		}
		current = current->next;
	}
}
