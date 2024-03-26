#include "../includes/minishell.h"

char	*trim_line(char *line, char *set)
{
	char	*str;
	int		len;

	str = ft_strtrim(line, set);
	len = ft_strlen(str);
    if (len >= 2 && ((str[len - 1] == '\"' && str[len - 2] == '\"')
		|| (str[len - 1] == '\'' && str[len - 2] == '\'')))
	{
		ft_bzero(&str[len - 2], 2);
	}
	return (str);
}

t_tokens	*tokenize(char *line)
{
	t_tokens	*tokens = NULL;
	char		*trimmed_line;

	trimmed_line = trim_line(line, WHITESPACE);
	if (!trimmed_line)
		return (NULL);
	if (check_syntax_errors(trimmed_line))
		return (NULL);
	tokens = get_tokens(trimmed_line);
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
		line = skip_whitespace_and_empty_quotes(line);
		if (*line == '\0')
			break ;
		if (*line == '\'')
			line = handle_single_quotes(line, current);
		else if (*line == '\"')
			line = handle_double_quotes(line, &current);
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

/* skip beginning quote, stdup everything inside,
create node for each word and each envp,
set pointer to current to last node created */
char	*handle_double_quotes(char *start, t_tokens **current)
{
	t_tokens	*previous;
	char		*line;

	start++;
	char *new_start = ft_strchr(start, '\"') + 1;
	if (*start == '\"')
		return (new_start);
	line = ft_strdup_delimiter_char(start, '\"');
	if (!line || *line == 0)
		return (error_message("double quotes"), NULL);
	while (*line != '\0')
	{
		if (*line == '$' && !ft_strchr(WHITESPACE, *(line + 1)))
			line = token_envp(line, *current);
		else if (*line == '$')
			line = token_word(line, *current, "\"");
		else
			line = token_word(line, *current, QUOTE_DELIMITER);
		previous = *current;
		*current = add_node_back(previous);
	}
	free(*current);
	previous->next = NULL;
	*current = previous;
	return (new_start);
}