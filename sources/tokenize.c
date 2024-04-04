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

void	count_pipes(t_shell *shell)
{
	t_tokens *current;

	current = shell->tokens;
	while (current != NULL)
	{
		if (current->type == PIPE)
			shell->pipe_nbr++;
		current = current->next;
	}
}

t_tokens	*tokenize(t_shell *shell)
{
	char		*trimmed_line;

	trimmed_line = trim_line(shell->line, WHITESPACE);
	if (!trimmed_line)
		return (NULL);
	if (check_syntax_errors(trimmed_line))
		return (NULL);
	shell->tokens = get_tokens(trimmed_line);
	free(trimmed_line);
	count_pipes(shell);
	if (!shell->tokens)
		shell->status = 1; // change to the right value
	return (shell->tokens);
}
// something here doesn't work when there is a space before a ending quote
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
		printf("line: %s\n", line);
		line = skip_whitespace(line);
		if (*line == '\0')
			break ;
		if (*line == '\'')
			line = handle_single_quotes(line, current);
		else if (*line == '\"')
			line = handle_double_quotes(line, current);
		else if (ft_strchr(SINGLE_TOKENS, *line) && ft_strchr(WHITESPACE, *(line + 1)))
			line = single_token(line, current);
		else if ((*line == '>' && *(line + 1) == '>' && ft_strchr(WHITESPACE, *(line + 2)))
			|| (*line == '<' && *(line + 1) == '<' && ft_strchr(WHITESPACE, *(line + 2))))
			line = double_token(line, current);
		else if (*line == '$' && ft_strchr(WHITESPACE, *(line + 1)))
			line = token_dollar(line, current);
		else if (*line == '$' && !ft_strchr(WHITESPACE, *(line + 1)))
			line = token_envp(line, current);
		else
			line = token_word(line, current, WHITESPACE_Q_D);
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
	if (ft_strchr(WHITESPACE, *start))
		token->space = 1;
	return (start);
}

char	*handle_double_quotes(char *start, t_tokens *token)
{
	char		*word;
	char		*dollar;

	word = ft_strdup_delimiter_char(++start, '\"');
	if (!word)
		return (error_message("token error: quotes have '\0' input"), NULL);
	token->value = word;
	dollar = ft_strchr(word, '$');
	if (dollar && !ft_strchr(WHITESPACE, *(dollar + 1)))
		token->type = ENV_VAR;
	else
		token->type = WORD;
	start += ft_strlen(word) + 1;
	if (ft_strchr(WHITESPACE, *start))
		token->space = 1;
	return (start);
}
