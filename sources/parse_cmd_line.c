#include "../includes/minishell.h"

void	redir_add_back(t_redir_node **head_redir_list, t_redir_node *new_node)
{
	t_redir_node *current;

	current = *head_redir_list;
	if (new_node == NULL)
		return ;
	if (!*head_redir_list)
		*head_redir_list = new_node;
	else
	{
		while (current->next != NULL)
			current = current->next;
		current->next = new_node;
	}
}

t_redir_node	*add_redir_node(t_tokens *current)
{
	t_redir_node	*new_node;

	new_node = (t_redir_node *)malloc(sizeof(t_redir_node));
	if (!new_node)
		return (error_message("malloc error: adding AST node"), NULL);
	new_node->type = current->type;
	new_node->file = NULL;
	new_node->delimiter = NULL;
	if (current->type == RE_INPUT || current->type == RE_OUTPUT || current->type == APPEND)
		new_node->file = current->next->value;
	else if (current->type == HEREDOC)
		new_node->delimiter = current->next->value;
	else
		error_message("Error adding redir node");
	new_node->next = NULL;
	return (new_node);
}

t_tree_node	*add_ast_node(t_tokens *tokens)
{
	t_tree_node	*new_node;

	new_node = (t_tree_node *)malloc(sizeof(t_tree_node));
	if (!new_node)
		return (error_message("malloc error: adding AST node"), NULL);
	new_node->type = tokens->type;
	new_node->cmd = NULL;
	new_node->right = NULL;
	new_node->left = NULL;

	return (new_node);
}

/* go through all (rest) tokens, take value, save in one big char** */
t_tree_node *parse_cmd(t_tokens *tokens_start, t_tokens *tokens_end)
{
	char **cmd_array;
	t_tree_node *cmd_node;
	t_tokens *current;
	int	i;
	int token_len;

	token_len = tokens_len(tokens_start, tokens_end);
	cmd_array = (char **)malloc(sizeof(char *) * (token_len + 1));
	if (!cmd_array)
		return (NULL);
	current = tokens_start;
	i = 0;
	while (i < token_len)
	{
		cmd_array[i] = current->value;
		i++;
		current = current->next;
	}
	cmd_array[i] = NULL;
	cmd_node = add_ast_node(tokens_start);
	cmd_node->cmd = cmd_array;
	return (cmd_node);
}

/* add type of redir and file/delimiter to a node in redir list
go through tokens until pipe/tokens_end, add new node for each redirect/file */
t_redir_node	*add_redir_list(t_tokens *tokens_start, t_tokens *tokens_end)
{
	t_redir_node	*new_node;
	t_redir_node	*head_redir_list;
	t_tokens		*current;

	current = tokens_start;
	head_redir_list = NULL;
	while ((current && current->next) && (current != tokens_end))
	{
		if (current->type >= RE_INPUT && current->type <= HEREDOC)
		{
			new_node = add_redir_node(current);
			redir_add_back(&head_redir_list, new_node);
			current = current->next;
		}
		current = current->next;
	}
	return (head_redir_list);
}

t_tree_node	*parse_execution(t_tokens *tokens_start, t_tokens *tokens_end)
{
	t_tree_node	*ast_node;
	t_tokens	*current;
	t_tokens	*previous;

	current = tokens_start;
	previous = NULL;
	while ((current && current->next) && (current != tokens_end))
	{
		if (current->type >= RE_INPUT && current->type <= HEREDOC)
		{
			ast_node = add_ast_node(current);
			ast_node->redir_list = add_redir_list(current, tokens_end);
			//delete tokens from token list
			if (current->previous) {
                current->previous->next = current->next->next;  // Skip redir & file node
                free(current);  // Free memory for current node
				free(current->next);
                current = previous->next;  // Move to the next node
            } else {
                tokens_start = current->next->next;  // Update tokens_start if deleting the first node
                free(current);  // Free memory for current node
				free(current->next);
                current = tokens_start;  // Move to the new first node
            }
			ast_node->left = parse_cmd(tokens_start, tokens_end);
			return (ast_node);
		}
		current = current->next;
		previous = current;
	}
	//if no redirection to the right
	ast_node = parse_cmd(tokens_start, current);
	return (ast_node);
}

t_tree_node	*parse_commandline(t_tokens *tokens_start)
{
	t_tree_node	*ast_head;
	t_tokens	*current;

	current = tokens_start;
	while (current && current->next)
	{
		if (current->type == PIPE)
		{
			ast_head = add_ast_node(current);
			ast_head->right = parse_commandline(current->next);
			ast_head->left = parse_execution(tokens_start, current);
			return (ast_head);
		}
		current = current->next;
	}
	// if no pipe to the right
	ast_head = parse_execution(tokens_start, current);
	return (ast_head);
}

// Function to print the AST tree and associated redirection lists
// Helper function to print spaces for indentation
void print_spaces(int count) {
    for (int i = 0; i < count; i++) {
        printf("    ");  // Adjust the number of spaces as needed
    }
}

// Function to print the AST tree and associated redirection lists with indentation
void print_ast_tree(t_tree_node *root, int level)
{
    if (root == NULL) {
        return;
    }

    // Print the node type with indentation based on the level
    print_spaces(level);
    printf("Node Type: %d\n", root->type);

    // Print command if available
    if (root->cmd != NULL) {
        print_spaces(level);
        printf("Command: ");
        for (int i = 0; root->cmd[i] != NULL; i++) {
            printf("%s ", root->cmd[i]);
        }
        printf("\n");
    }

    // Print redirection list if available
    if (root->redir_list != NULL) {
        print_spaces(level);
        printf("Redirection List:\n");
        print_redir_list(root->redir_list, level + 1);
    }

    // Recursively print left and right subtrees with increased indentation
    print_spaces(level);
    printf("Left subtree:\n");
    print_ast_tree(root->left, level + 1);

    print_spaces(level);
    printf("Right subtree:\n");
    print_ast_tree(root->right, level + 1);
}

// Function to print the redirection list with indentation
void print_redir_list(t_redir_node *head_redir_list, int level)
{
    t_redir_node *current = head_redir_list;

    while (current != NULL) {
        print_spaces(level);
        printf("Type: %d, ", current->type);
        if (current->file != NULL) {
            printf("File: %s", current->file);
        }
        if (current->delimiter != NULL) {
            printf(", Delimiter: %s", current->delimiter);
        }
        printf("\n");
        current = current->next;
    }
}
