#include "../includes/minishell.h"

void	print_tokens(t_tokens *head)
{
	t_tokens	*current = head;
	int			index = 1;

	while (current != NULL)
	{
		printf("+----------------------------------+\n");
		printf("| Node %d                           |\n", index);
		printf("+----------------------------------+\n");
		printf("| Value: %-25s |\n", current->value);
		printf("| Type: %-26d |\n", current->type);

		if (current->previous != NULL)
			printf("| Previous: Node %-4d (%-10s)|\n", index - 1, current->previous->value);
		else
			printf("| Previous: %-24s|\n", "NULL");

		if (current->next != NULL)
			printf("| Next: Node %-8d (%-10s)|\n", index + 1, current->next->value);
		else
			printf("| Next: %-28s|\n", "NULL");
		printf("+----------------------------------+\n\n");
		current = current->next;
		index++;
	}
}

void	print_spaces(int count)
{
	for (int i = 0; i < count; i++)
		printf("    ");
}

void	print_ast_tree(t_tree_node *root, int level)
{
	if (root == NULL)
		return;
	printf("\n");
	print_spaces(level);
	printf("  Node Type: %d\n", root->type);
	if (root->cmd != NULL)
	{
		print_spaces(level);
		printf("	Command: ");
		for (int i = 0; root->cmd[i] != NULL; i++)
			printf("%s ", root->cmd[i]);
		printf("\n");
	}
	if (root->redir_list != NULL)
	{
		print_spaces(level);
		printf("  Redirection List:\n");
		print_redir_list(root->redir_list, level + 1);
	}
	print_spaces(level);
	printf("  Left subtree:\n");
	print_ast_tree(root->left, level + 1);
	
	print_spaces(level);
	printf("  Right subtree:\n");
	print_ast_tree(root->right, level + 1);
	printf("\n");
}

void	print_redir_list(t_redir_node *head_redir_list, int level)
{
	t_redir_node *current = head_redir_list;

	while (current != NULL)
	{
		print_spaces(level);
		if (current->file != NULL)
			printf("	File: %s", current->file);
		if (current->delimiter != NULL)
			printf(", 	Delimiter: %s", current->delimiter);
		printf("\n");
		current = current->next;
	}
}