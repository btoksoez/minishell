#include "../includes/minishell.h"

char	*ft_strdup_delimiter_char(const char *s, char delimiter)
{
	int		i;
	char	*dup;

	i = 0;
	while (s[i] != delimiter)
		i++;
	dup = malloc(sizeof(char) * (i + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (s[i] != delimiter)
	{
		((unsigned char *)dup)[i] = ((unsigned char *)s)[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*ft_strdup_delimiter_string(const char *s, char *delimiter)
{
	int		i;
	char	*dup;

	i = 0;
	while (!ft_strchr(delimiter, s[i]))
		i++;
	dup = malloc(sizeof(char) * (i + 1));
	if (dup == NULL)
		return (NULL);
	i = 0;
	while (!ft_strchr(delimiter, s[i]))
	{
		((unsigned char *)dup)[i] = ((unsigned char *)s)[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

/* checks whether the current and next char are empty quotes or whitespace and skips them,
else returns current position */
char	*skip_whitespace_and_empty_quotes(char *line)
{
	while ((ft_strchr(WHITESPACE, *line)
		|| (*line == '\'' && *(line + 1) == '\'')
		|| (*line == '\"' && *(line + 1) == '\"')) && *line)
	{
		if (*line == '\'' || *line == '\"')
			line += 2;
		else
			line++;
	}
	return line;
}

char	*skip_whitespace(char *line)
{
	while (ft_strchr(WHITESPACE, *line) && *line)
		line++;
	return line;
}

t_tokens	*add_node_back(t_tokens *previous)
{
	t_tokens	*new;

	new = (t_tokens *)malloc(sizeof(t_tokens));
	if (!new)
		return (error_message("malloc error: add token"), NULL);
	new->previous = previous;
	new->next = NULL;
	previous->next = new;
	new->value = NULL;
	new->type = 0;
	return (new);
}

t_tokens	*token_init(void)
{
	t_tokens	*new;

	new = (t_tokens *)malloc(sizeof(t_tokens));
	if (!new)
		return (error_message("malloc error: token init"), NULL);
	new->previous = NULL;
	new->next = NULL;
	new->value = NULL;
	new->type = 0;
	return (new);
}

/* prints tokens for test purposes */
void print_tokens(t_tokens *head)
{
    t_tokens *current = head;
    int index = 1;

    while (current != NULL) {
        printf("+----------------------------------+\n");
        printf("| Node %d                           |\n", index);
        printf("+----------------------------------+\n");
        printf("| Value: %-25s |\n", current->value);
        printf("| Type: %-26d |\n", current->type);

        // Print previous pointer
        if (current->previous != NULL) {
            printf("| Previous: Node %-4d (%-10s) |\n", index - 1, current->previous->value);
        } else {
            printf("| Previous: %-24s |\n", "NULL");
        }

        // Print next pointer
        if (current->next != NULL) {
            printf("| Next: Node %-8d (%-10s) |\n", index + 1, current->next->value);
        } else {
            printf("| Next: %-28s |\n", "NULL");
        }

        printf("+----------------------------------+\n\n");

        current = current->next;
        index++;
    }
}
