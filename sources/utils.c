#include "../includes/minishell.h"

char	*skip_whitespace(char *line)
{
	while (ft_strchr(WHITESPACE, *line))
		line++;
	return (line);
}
char	*ft_strdup_delimiter_char(const char *s, char delimiter)
{
	int		i;
	char	*dup;

	i = 0;
	while (s[i] != delimiter)
		i++;
	dup = malloc(sizeof(char) * (i + 1));
	if (dup == NULL)
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
// int	main(void)
// {
// 	char	str[] = "hello   ";
// 	char	*dup = ft_strdup_whitespace(str);

// 	printf("Length: %d\n", ft_strlen(dup));
// 	printf("My function: %s\n", dup);
// 	free(dup);
// 	return (0);
// }
