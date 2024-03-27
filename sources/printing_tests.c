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

static void print_indent(int level)
{
    for (int i = 0; i < level * 4; i++) {
        printf(" ");
    }
}

void print_tree(t_tree_node *root, int level)
{
    if (root == NULL) {
        return;
    }

    print_indent(level);
    printf("Node Type: %d\n", root->type);

    if (root->type == CMD) {
        print_indent(level);
        printf("Command: %s\n", root->cmd);
    }

    // Print arguments list
    t_args *args = root->args;
    print_indent(level);
    printf("Arguments:\n");
    while (args != NULL) {
        print_indent(level);
        printf("%s\n", args->arg);
        args = args->next;
    }

    // Print redirection list
    t_redir_list *redir = root->redir_list;
    print_indent(level);
    printf("Redirection List:\n");
    while (redir != NULL) {
        print_indent(level);
        printf("Type: %d, File: %s\n", redir->type, redir->file);
        redir = redir->next;
    }

    print_indent(level);
    printf("Left subtree:\n");
    print_tree(root->left, level + 1);

    print_indent(level);
    printf("Right subtree:\n");
    print_tree(root->right, level + 1);
}