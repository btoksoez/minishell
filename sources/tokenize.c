#include "../includes/minishell.h"

t_tokens	*tokenize(char *line)
{
	t_tokens	*tokens = NULL;
	char		*trimmed_line;

	trimmed_line = ft_strtrim(line, WHITESPACE);
	if (!trimmed_line)
		return (NULL);
	if (check_syntax_errors(trimmed_line))
		return (NULL);
	tokens = get_tokens(trimmed_line);
	if (!tokens)
		free(line);
	free(trimmed_line);
	return (tokens);
}

t_tokens	*get_tokens(char *line)
{
	t_tokens	*head;
	t_tokens	*current;
	t_tokens	*previous;

	head = token_init();
	current = head;
	previous = NULL;
	if (!line || *line == '\0')
		return (NULL);
	while (*line != '\0')
	{
		line = skip_whitespace(line);
		if (*line == '\'')
			line = handle_single_quotes(line, current);
		else if (ft_strchr(SINGLE_TOKENS, *line) && ft_strchr(WHITESPACE, *(line + 1)))
			line = single_token(line, current);
		else if ((*line == '>' && *(line + 1) == '>') || (*line == '<' && *(line + 1) == '<'))
			line = double_token(line, current);
		else if (*line == '$' && !ft_strchr(WHITESPACE, *(line + 1)))
			line = token_envp(line, current);
		else
			line = token_word(line, current, WHITESPACE);
		previous = current;
		current = add_node_back(previous);
	}
	free(current);
	previous->next = NULL;
	return (head);
}

char	*handle_single_quotes(char *start, t_tokens *token)
{
	char	*word;

	word = ft_strdup_delimiter_char(++start, '\'');
	if (!word)
		return (error_message("token error: quotes have '\0' input"), NULL);
	token->value = word;
	token->type = WORD;
	start += ft_strlen(word) + 1;
	return (start);
}

// char	*handle_double_quotes(char *start, t_tokens *token)
// {
// 	// while (*start != '\"')
// 	// {

// 	// }
// 	//while start is not closing quotes
// 		//strdup until you encounter '$' or '\"'
// 			//if word is NULL or '\0', free + delete ->means either empty quotes or $ is first thing
// 			//if not empty make new node with word
// 		//if start + ft_strlen(word) == '$' -> next one is envp
// 		//strdup until whitespace
// 			//make new node
// 		//repeat
// }