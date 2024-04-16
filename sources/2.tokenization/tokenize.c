#include "../../includes/minishell.h"

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

void	remove_empty_tokens(t_tokens **tokens)
{
	t_tokens	*current;
	t_tokens	*temp;

	if (!tokens || !(*tokens))
		return ;
	current = *tokens;
	temp = NULL;
	while (current)
	{
		if (current->type == 6 && ft_strcmp("", current->value) == 0)
		{
			temp = current->next;
			del_token(tokens, current);
			current = temp;
		}
		else
			current = current->next;
	}
}

/* concetonates all tokens if there is no space between them */
void	remove_spaces(t_tokens **tokens)
{
	t_tokens	*current;
	int			flag;

	if (!tokens || !(*tokens))
		return ;
	current = *tokens;
	flag = 0;
	while (current && current->next)
	{
		while ((current->type ==  5 || current->type == 6) && (current->next->type == 5 || current->next->type == 6 ) && current->space == 0)
		{
			flag = 0;
			current->value = ft_strjoin_free(current->value, current->next->value);
			if (current->next->space == 1)
				flag = 1;
			del_token(tokens, current->next);
			if (!current->next || flag == 1)
				break ;
		}
		current = current->next;
	}
}

void	pre_parse_tokens(t_tokens *tokens)
{
	t_tokens	*current;

	if (!tokens)
		return ;
	current = tokens;
	while (current && current->next)
	{
		if (current->type == RE_INPUT && current->next->type == RE_INPUT)
		{
			current->type = HEREDOC;
			del_token(&tokens, current->next);
		}
		else if (current->type == RE_OUTPUT && current->next->type == RE_OUTPUT)
		{
			current->type = APPEND;
			del_token(&tokens, current->next);
		}
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
	{
		shell->status = 2;
		return (NULL);
	}
	shell->tokens = get_tokens(trimmed_line);
	pre_parse_tokens(shell->tokens);
	free(trimmed_line);
	count_pipes(shell);
	return (shell->tokens);
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
		return (free(current), NULL);
	while (*line != '\0')
	{
		line = skip_whitespace(line);
		if (*line == '\0')
			break ;
		if (*line == '\'')
			line = handle_single_quotes(line, current);
		else if (*line == '\"')
			line = handle_double_quotes(line, current);
		else if (ft_strchr(SINGLE_TOKENS, *line))
			line = single_token(line, current);
		else if (*line == '$' && ft_strchr(WHITESPACE, *(line + 1)))
			line = token_dollar(line, current);
		else if (*line == '$' && !ft_strchr(WHITESPACE, *(line + 1)))
			line = token_envp(line, current);
		else if (*line == '?')
			line = token_word(line, current, WHITESPACE_Q_D_N);
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
		return (error_message("token error: quotes have '\0' input", NULL), NULL);
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
		return (error_message("token error: quotes have '\0' input", NULL), NULL);
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
