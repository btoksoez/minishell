#include "../../includes/minishell.h"

t_tree_node	*add_ast_node(void)
{
	t_tree_node	*new_node;

	new_node = (t_tree_node *)malloc(sizeof(t_tree_node));
	if (!new_node)
		return (error_message("malloc error: adding cmd node", NULL), NULL);
	new_node->type = PHANTOM;
	new_node->cmd = NULL;
	new_node->right = NULL;
	new_node->left = NULL;
	new_node->redir_list = NULL;
	new_node->args = NULL;
	new_node->builtin = NULL;
	return (new_node);
}

void	add_redir_list(t_redir_list **head, t_token_type type, char *filename)
{
	t_redir_list	*new;
	t_redir_list	*current;

	current = NULL;
	new = (t_redir_list *)malloc(sizeof(t_redir_list));
	if (!new)
		return (error_message("malloc error redir list", NULL));
	if (!filename)
		return (error_message("not a valid file", NULL));	//might have to handle this as proper error
	new->type = type;
	new->file = filename;
	new->next = NULL;
	if (!*head)
		*head = new;
	//loop to the last element
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new;
	}
	//check whether filename not null, else error
	//if head is null, create new node and return as head
	//otherwise create new node and append to head
}

/* add arg to args */
void	add_arg(t_args **args, t_tokens *current)
{
	t_args	*new_arg;
	t_args	*cur_arg;

	cur_arg = NULL;
	new_arg = (t_args *)malloc(sizeof(t_args));
	if (!new_arg)
		return (error_message("malloc error args", NULL));
	// if (current->value != NULL)
	new_arg->arg = current->value;
	// else
	// 	new_arg->arg = NULL;
	new_arg->next = NULL;
	new_arg->space = current->space;
	if (!*args)
		*args = new_arg;
	else
	{
		cur_arg = *args;
		while (cur_arg->next)
			cur_arg = cur_arg->next;
		cur_arg->next = new_arg;
	}
}


t_tree_node	*parse_cmd(t_tokens *tokens_start, t_tokens *tokens_end)
{
	//tokens_end will either point to the pipe or to NULL (end of tokens)
	t_tree_node	*cmd_node;
	t_tokens	*current;

	current = tokens_start;
	//adds phantom node for now
	cmd_node = add_ast_node();
	//go through tokens
	while (current != tokens_end)	//will not process tokens_end
	{
		//if it's a redirection add to list
		if (current->type >= RE_INPUT && current->type <= HEREDOC)
		{
			add_redir_list(&(cmd_node->redir_list), current->type, current->next->value);
			//do current = current->next to skip both redirction and file token_nodes
			current = current->next;
		}
		//first word that is not a redirecation is the cmd/builtin, add it to cmd->type
		else if (cmd_node->type == PHANTOM)
		{
			cmd_node->builtin = builtin_arr(current->value);	//will be null if current->value is not a builtin cmd
			if (cmd_node->builtin != NULL)
				cmd_node->type = BUILTIN;
			else
			{
				cmd_node->type = CMD;
				cmd_node->cmd = current->value;
			}
		}
		//else add it as an arg
		else
			add_arg(&(cmd_node->args), current);
		current = current->next;
	}
	return (cmd_node);
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
			ast_head = add_ast_node();
			ast_head->type = PIPE_TREE;
			ast_head->right = parse_commandline(current->next);
			ast_head->left = parse_cmd(tokens_start, current);
			return (ast_head);
		}
		current = current->next;
	}
	ast_head = parse_cmd(tokens_start, current->next);
	return (ast_head);
}
