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

char	*skip_whitespace(char *line)
{
	while (ft_strchr(WHITESPACE, *line))
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
