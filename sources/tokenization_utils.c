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
	while (!ft_strchr(delimiter, s[i]) && s[i])
		i++;
	dup = malloc(sizeof(char) * (i + 1));
	if (dup == NULL)
		return (NULL);
	i = 0;
	while (s[i] && !ft_strchr(delimiter, s[i]))
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
	return (line);
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

int	tokens_len(t_tokens *tokens_start, t_tokens *tokens_end)
{
	int	len;
	t_tokens	*current;

	current = tokens_start;
	len = 0;
	while (current != tokens_end && current->next)
	{
		current = current->next;
		len++;
	}
	return (len);
}
int	args_len(t_args *args)
{
	int	len;

	len = 0;
	if (!args)
		return (len);
	while (args)
	{
		args = args->next;
		len++;
	}
	return (len);
}
