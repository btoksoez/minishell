#include "../includes/minishell.h"

/* go until whitespace encountered, save as value in envp */
char	*token_envp(char *start, t_tokens *token)
{
	char	*word;

	word = ft_strdup_delimiter_string(++start, WHITESPACE);	//skip dollar sign
	if (!word)
		return (error_message("strdup token envp"), NULL);
	token->value = word;
	token->type = ENV_VAR;
	start += ft_strlen(word) + 1;
	return (start);
}

// char	*handle_quotes(char *start, t_tokens *token)
// {
// 	char	*word;
// 	//check if single or double quotes
// 	//add as word?
// 	//if double quotes and if $ call token_envp function?
// 	if (start == '\'')
// 	{
// 		word = ft_strdup_delimiter_char(++start, '\'');
// 		if (!word)
// 			return (error_message("strdup quotes"), NULL);
// 	}
// 	else if (start++ == '\"')
// 	{
// 		while (start != '\"')
// 		{

// 		}
// 		//while start is not closing quotes
// 			//strdup until you encounter '$' or '\"'
// 				//if word is NULL or '\0', free + delete ->means either empty quotes or $ is first thing
// 				//if not empty make new node with word
// 			//if start + ft_strlen(word) == '$' -> next one is envp
// 			//strdup until whitespace
// 				//make new node
// 			//repeat
// 	}
// 	else
// 		return (error_message("quotes wrong param"), NULL);
// 	token->value = word;
// 	token->type = WORD;
// 	start += ft_strlen(word) + 1;
// 	return (start);
// }

t_tokens	*add_node_back(t_tokens *previous)
{
	t_tokens	*new;

	new = (t_tokens *)malloc(sizeof(t_tokens));
	if (!new)
		return (error_message("add token node malloc"), NULL);
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
		return (error_message("token init node malloc"), NULL);
	new->previous = NULL;
	new->next = NULL;
	new->value = NULL;
	new->type = 0;
	return (new);
}

/*takes start of line, skips until word ends (until whitespace is encountered)
null terminates word and puts it into value, puts word as type, returns end of word as new start*/
char	*token_word(char *start, t_tokens *token, char *delimiter)
{
	char	*word;

	word = ft_strdup_delimiter_string(start, delimiter);
	if (!word)
		return (error_message("strdup token word"), NULL);
	token->value = word;
	token->type = WORD;
	start += ft_strlen(word);
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
	else
		return (error_message("single token error"), NULL);
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
		return (error_message("double token error"), NULL);
	start += 2;
	return (start);
}

/* prints tokens for test purposes */
void print_tokens(t_tokens *head) {
    t_tokens *current = head;
    int index = 1;

    while (current != NULL) {
        printf("Token %d: Type: %d, Value: %s\n", index, current->type, current->value);

        // Print the link arrows and pointers
        if (current->previous == NULL) {
            printf("NULL <- ");
        } else {
            printf("Token %d <- ", index - 1);
        }

        printf("Token %d ", index);

        if (current->next == NULL) {
            printf("-> NULL\n");
        } else {
            printf("-> Token %d\n", index + 1);
        }

        current = current->next;
        index++;
    }
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
		line = skip_whitespace(line);
		// if (*line == '\'' || line == '\"')
		// 	line = handle_quotes(line, current);
		if (ft_strchr(SINGLE_TOKENS, *line))
			line = single_token(line, current);
		else if (*line == '>>' || *line == '<<')
			line = double_token(line, current);
		else if (*line == '$')
			line = token_envp(line, current);
		else
			line = token_word(line, current, WHITESPACE);
		previous = current;
		current = add_node_back(previous);
	}
	current = NULL;
	return (head);

	//go through each character
	//separate by whitespace, but not if inside quotes
		//call a function that returns whatever is inside quotes as a word and skips to the ending of the quote?
	//classify what token it is and put it in the node
	//return list
}

/*free tokens, might need changes */
void free_tokens(t_tokens *head) {
    t_tokens *current = head;
    t_tokens *next_node;

    while (current != NULL) {
        next_node = current->next;  // Save the next node before freeing the current one
        if (current->value != NULL)
			free(current->value);  // Free the value (assuming it's dynamically allocated)
        free(current);  // Free the current node
        current = next_node;  // Move to the next node
    }
}


/* main for testing the token function */
int main() {
    // Assume you have a function get_tokens that creates and returns the token list
    char line[] = "ls -l \"file name\" $HOME > output.txt";
    t_tokens *token_list = get_tokens(line);

    if (token_list != NULL) {
        // Print the token list
        print_tokens(token_list);
    }

    // Free the token list memory (assuming you have a function to free nodes)
    free_tokens(token_list);  // Implement a function to free allocated memory

    return 0;
}
