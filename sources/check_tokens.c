#include "../includes/minishell.h"

bool	check_tokens(t_tokens *tokens)
{
	//check if after redirection is a word
	t_tokens *current;

	current = tokens;
	while (current)
	{
		if (current->type >= RE_INPUT && current->type <= HEREDOC)
		{
			if (!current->next)
				return (error_message("syntax error near `\\n'"), false);
			//add to check for word
		}
		current = current->next;
	}
	return (true);
}
