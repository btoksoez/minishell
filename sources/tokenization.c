#include "../includes/minishell.h"


t_token	*tokenize(char *line)
{
	t_token *tokens;
	char	*trimmed_line;

	trimmed_line = ft_strtrim(line, WHITE_SPACES);
	if (!trimmed_line)
		return (NULL);
	if (check_syntax_errors(trimmed_line))		// check syntax needs to be completed
		return (NULL);
	tokens = get_tokens(trimmed_line);			// get tokens needs to be completed
	free(trimmed_line);
	return (tokens);
}