#include "../includes/minishell.h"

char	*token_envp(char *start, t_tokens *token)
{
	//go until whitespace encountered, save as value in envp (could call word function?)
}

char	*handle_quotes(char *start, t_tokens *token)
{
	//check if single or double quotes
	//add as word?
	//if double quotes and if $ call token_envp function?
}

t_tokens	*add_node_back(t_tokens *previous)
{
	t_tokens	*new;

	new = (t_tokens *)malloc(sizeof(t_tokens));
	if (!new)
		return (NULL);
	new->previous = previous;
	new->next = NULL;
	previous->next = new;
	new->value = NULL;
	new->type = 0;
	return (new);
}
/* not sure if this is necessary
should initialize new node 		*/

t_tokens	*token_init(void)
{
	t_tokens	*new;

	new = (t_tokens *)malloc(sizeof(t_tokens));
	if (!new)
		return (NULL);
	new->previous = NULL;
	new->next = NULL;
	new->value = NULL;
	new->type = 0;
	return (new);
}

/*takes start of line, skips until word ends (until whitespace is encountered)
null terminates word and puts it into value, puts word as type, returns end of word as new start*/
char	*token_word(char *start, t_tokens *token)
{
	char	*word;
	char	*word_start;

	word_start = word;
	while (!ft_strchr(WHITESPACE, *start))
	{
		*word = *start;
		word++;
		start++;
	}
	*word = '\0';
	token->value = word_start;
	token->type = WORD;
	return (start);
}

/*
should take start of line,
put token as type, return end of token
*/
char	*single_token(char *start, t_tokens *token)
{
	if (*start == '<')
		token->type = RE_INPUT;
	else if (*start == '>')
		token->type = RE_OUTPUT;
	else if (*start == '|')
		token->type = PIPE;
	else if (*start == '$')
		token->type = ENV_VAR;
		//need to handle what comes afterwards?
	else
		error_message("single token error");
	start++;
	return (start);
}

/*
should take start of line,
put token as type, return end of token
*/
char	*double_token(char *start, t_tokens *token)
{
	if (*start == '<<')
		token->type = HEREDOC;
	else if (*start == '>>')
		token->type = APPEND;
	else
		error_message("double token error");
	start += 2;
	return (start);
}

/* should return a list of token nodes that contains all tokens, ready to be passed to tree? */
t_tokens *get_tokens(char *line)
{
	t_tokens	*head;
	t_tokens	*current;
	t_tokens	*previous;
	head = token_init();
	current = head;
	previous = NULL;
	while (*line)
	{
		if (*line == '\'' || line == '\"')
			line = handle_quotes(line, current);
		else if (ft_strchr(SINGLE_TOKENS, *line))
			line = single_token(line, current);
		else if (*line == '>>' || *line == '<<')
			line = double_token(line, current);
		else if (*line == '$')
			line = token_envp(line, current);
		else
			line = token_word(line, current);
		previous = current;
		current = add_node_back(previous);
	}
	if (current != NULL)
		current->next = NULL;
	return (head);

	//go through each character
	//separate by whitespace, but not if inside quotes
		//call a function that returns whatever is inside quotes as a word and skips to the ending of the quote?
	//classify what token it is and put it in the node
	//return list
}
