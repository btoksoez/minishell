#include "../includes/minishell.h"

void	check_operator_token(char *line, t_token **tokens)
{
	(void)line;
	(void)tokens;
}

void	check_word_token(char *line, t_token **tokens)
{
	(void)line;
	(void)tokens;
}

void	get_tokens(char *line, t_token *tokens)
{
	while (*line)
	{
		while (*line && ft_strchr(WHITE_SPACES, *line))
			line++;
		if (ft_strchr(TOKENS, *line))
			check_operator_token(line, &tokens);
		else
			check_word_token(line, &tokens);
		line++;
	}
}

void	tokenize(char *line, t_token *tokens)
{
	char	*trimmed_line;

	trimmed_line = ft_strtrim(line, WHITE_SPACES);
	if (!trimmed_line)
		return ;
	if (check_syntax_errors(trimmed_line))		// check syntax needs to be completed
		return ;
	get_tokens(trimmed_line, tokens);			// get tokens needs to be completed
	free(trimmed_line);
}
