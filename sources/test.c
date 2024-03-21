#include "../includes/minishell.h"

char	*skip_whitespace(char *line)
{
	while (ft_strchr(WHITESPACE, *line))
		line++;
	return (line);
}
int main()
{
	char *line = "   helllo  frf";
	while (*line)
	{
		line = skip_whitespace(line);
		printf("Line: %s\n", line);
		line++;
	}
}
