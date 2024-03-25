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
	new_node->args = tokens->value;

	return (new_node);
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
	t_tokens 	*current;

	current = tokens_start;
	ast_node->right = NULL;
	ast_node->left = NULL;
	while ((current && current->next) && (current != tokens_end))
	{
		if (current->type >= RE_INPUT && current->type <= HEREDOC)
		{
			ast_node = add_ast_node(current);
			ast_node->right = add_redir_list(current, tokens_end);
			//delete tokens from token list
			ast_node->left = parse_cmd(tokens_start, current);
			return (ast_node);
		}
		current = current->next;
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
	ast_head->right = NULL;
	ast_head->left = NULL;
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

// void print_ast(t_tree_node *node, int level)
// {
//     if (node == NULL) return;

//     // Print the current node
//     printf("%*s", level * 4, ""); // Adjust indentation based on the level
//     switch (node->type) {
//         case PIPE:
//             printf("PIPE\n");
//             break;
//         case RE_INPUT:
//             printf("RE_INPUT\n");
//             break;
//         case RE_OUTPUT:
//             printf("RE_OUTPUT\n");
//             break;
//         case APPEND:
//             printf("APPEND\n");
//             break;
//         case HEREDOC:
//             printf("HEREDOC\n");
//             break;
//         case WORD:
//             printf("WORD: %s\n", node->args);
//             break;
//         case ENV_VAR:
//             printf("ENV_VAR: %s\n", node->args);
//             break;
//         default:
//             printf("Unknown type\n");
//             break;
//     }

//     // Recursively print left and right subtrees
//     print_ast(node->left, level + 1);
//     print_ast(node->right, level + 1);
// }

// int main() {
//     // Assuming you have an AST root node named ast_root
//     t_tree_node *ast_root = parse_commandline(/* your tokens */);

//     // Print the AST
//     printf("Abstract Syntax Tree:\n");
//     print_ast(ast_root, 0);

//     return 0;
// }
