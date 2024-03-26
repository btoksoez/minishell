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
	new_node->redir_list = NULL;
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

// static void	free_redir_nodes(t_tokens *prev, t_tokens *curr, t_tokens *token_s)
// {
// 	if (curr->previous)
// 	{
// 		curr->previous->next = curr->next->next;  		// Skip redir & file node
// 		free(curr);  									// Free memory for current node
// 		free(curr->next);
// 		curr = prev->next;  							// Move to the next node
// 	}
// 	else
// 	{
// 		token_s = curr->next->next;  					// Update tokens_start if deleting the first node
// 		free(curr); 									// Free memory for current node
// 		free(curr->next);
// 		curr = token_s;  								// Move to the new first node
// 	}
// }

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
			// free_redir_nodes(previous, current, tokens_start);
			if (current->previous)
			{
				current->previous->next = current->next->next;
				free(current);
				free(current->next);
				current = previous->next;
			}
			else
			{
				tokens_start = current->next->next;
				free(current);
				free(current->next);
				current = tokens_start;
			}
			ast_node->left = parse_cmd(tokens_start, tokens_end);
			return (ast_node);
		}
		current = current->next;
		previous = current;
	}
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
	ast_head = parse_execution(tokens_start, current);
	return (ast_head);
}
