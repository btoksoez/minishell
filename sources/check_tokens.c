#include "../includes/minishell.h"

bool	check_tokens(t_tokens *tokens)
{
	t_tokens *current;

	current = tokens;
	while (current)
	{
		//check if after redirection is a word
		if (current->type >= RE_INPUT && current->type <= HEREDOC)
		{
			if (!current->next)
				return (error_message("syntax error near `\\n'"), true);
			else if (current->next->type == PIPE)
				return (error_message("syntax error near `|'"), true);
			else if (current->next->type == RE_INPUT)
				return (error_message("syntax error near `<'"), true);
			else if (current->next->type == RE_OUTPUT)
				return (error_message("syntax error near `>'"), true);
			else if (current->next->type == APPEND)
				return (error_message("syntax error near `>>'"), true);
			else if (current->next->type == HEREDOC)
				return (error_message("syntax error near `<<'"), true);
		}
		current = current->next;
	}
	return (false);
}