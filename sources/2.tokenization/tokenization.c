#include "../../includes/minishell.h"

char	*token_word(char *start, t_tokens *token, char *delimiter)
{
	char	*word;

	word = ft_strdup_delimiter_string(start, delimiter);
	if (!word)
		return (error_message("token error: no word"), NULL);
	token->value = word;
	token->type = WORD;
	start += ft_strlen(word);
	if (ft_strchr(WHITESPACE, *start))
		token->space = 1;
	return (start);
}

char	*token_qm(char *start, t_tokens *token)
{
	char	*word;

	word = ft_strdup("\n");
	if (!word)
		return (error_message("token error: no word"), NULL);
	token->value = word;
	token->type = WORD;
	start += ft_strlen(word);
	if (ft_strchr(WHITESPACE, *start))
		token->space = 1;
	return (start);
}

char	*token_dollar(char *start, t_tokens *token)
{
	char	*word;

	word = ft_strdup_delimiter_string(start, WHITESPACE);
	if (!word)
		return (error_message("token error: no word"), NULL);
	token->value = word;
	token->type = WORD;
	start += ft_strlen(word);
	if (ft_strchr(WHITESPACE, *start))
		token->space = 1;
	return (start);
}

char	*single_token(char *start, t_tokens *token)
{
	if (*start == '<')
		token->type = RE_INPUT;
	else if (*start == '>')
		token->type = RE_OUTPUT;
	else if (*start == '|')
		token->type = PIPE;
	else
		return (error_message("token error: no single token"), NULL);
	start++;
	return (start);
}

char	*double_token(char *start, t_tokens *token)
{
	if (*start == '<' && *(start + 1) == '<')
		token->type = HEREDOC;
	else if (*start == '>' && *(start + 1) == '>')
		token->type = APPEND;
	else
		return (error_message("token error: no double token"), NULL);
	start += 2;
	return (start);
}

char	*token_envp(char *start, t_tokens *token)
{
	char	*word;

	if (*(start + 1) == '?')
		word = ft_strdup("$?");
	else if (*(start + 1) == '$')
		word = ft_strdup("$$");
	else
		word = ft_strdup_delimiter_string(start, WHITESPACE_QUOTES);
	if (!word)
		return (error_message("token error: envp whitespaces after $"), NULL);
	token->value = word;
	token->type = ENV_VAR;
	start += ft_strlen(word);
	if (ft_strchr(WHITESPACE, *start))
		token->space = 1;
	return (start);
}
