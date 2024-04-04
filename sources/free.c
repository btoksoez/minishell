#include "../includes/minishell.h"

void free_redir_list(t_redir_list *redir_list)
{
    t_redir_list *current = redir_list;
    t_redir_list *temp;

    while (current != NULL) {
        temp = current;
        current = current->next;

        free(temp->file);   // Free the redirection file
        free(temp);         // Free the redirection node itself
    }
}

void free_args(t_args *args)
{
    t_args *current = args;
    t_args *temp;

    while (current != NULL) {
        temp = current;
        current = current->next;

        free(temp->arg);    // Free argument string
        free(temp);         // Free argument node itself
    }
}

void free_tree(t_tree_node *node)
{
    if (node == NULL) {
        return;
    }
    free_tree(node->left);      // Free left subtree
    free_tree(node->right);     // Free right subtree
    free_redir_list(node->redir_list);  // Free redirection list
    free_args(node->args);      // Free arguments list

    free(node->cmd);            // Free command string
    free(node);                 // Free the node itself
}

void	free_tokens(t_tokens *head)
{
	t_tokens *current = head;
	t_tokens *next_node;

	while (current != NULL)
	{
		next_node = current->next;
		if (current->value != NULL)
			free(current->value);
		free(current);
		current = next_node;
	}
}
