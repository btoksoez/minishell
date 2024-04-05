#include "../includes/minishell.h"

char	*get_env(char *str)
{
	char	*env_value;

	if (*str == '?')
		return ("$?");
	else if (*str == '$')
		return ("$$");
	env_value = getenv(str);
	if (!env_value)
		env_value = "";
	return (env_value);
}

/* finds environemnt variable and returns address to its $
will return NULL if no envp found */
char	*find_env(char *str)
{
	while (*str)
	{
		if (*str == '$' && *(str + 1) && !ft_strchr(WHITESPACE, *(str + 1)))
			return (str);
		str++;
	}
	return (NULL);
}

/* mallocs the string from s until envp
return null if nothing in s before envp*/
char	*ft_strdup_until(char *s)
{
	int		i;
	int		len;
	char	*dup;
	char	*end;

	end = find_env(s);
	if (end == NULL)
		return (s);
	len = end - s;
	if (len < 1)
		return (NULL);
	dup = malloc(sizeof(char) * (len + 1));
	if (dup == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		((unsigned char *)dup)[i] = ((unsigned char *)s)[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

void	free_strs(char *s1, char *s2, char *s3)
{
	if (s1)
		free(s1);
	if (s2)
		free(s2);
	if (s3)
		free(s3);
}
