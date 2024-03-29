#include "../includes/minishell.h"


char	*replace_env(char *str)
{
	char *env_value;

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

char	*find_and_replace(char *org_str)
{
	char		*new_str;
	char		*str_before;
	char		*str_env;
	char		*result;

	result = ft_strdup("");
	while (*org_str)
	{
		str_before = ft_strdup_until(org_str);
		org_str = find_env(org_str);	//skip until env
		if (!org_str)
		{
			result = ft_strjoin(result, str_before);	// No more '$' found, append remaining string and break
			break;
		}
		str_env = ft_strdup_delimiter_string(++org_str, WHITESPACE_DOLLAR);	//malloc from start env until whitespace, dollar or end of org_str
		new_str = ft_strjoin(str_before, replace_env(str_env));	//replaces env with its value
		org_str += ft_strlen(str_env);
		result = ft_strjoin(result, new_str);
		free(str_before);
		free(str_env);
		free(new_str);
	}
	return (result);
}

/* expands environment variables in the token list,
replacing them by their value or by a empty string (\0\0) */
void	expand(t_tokens *tokens)
{
	t_tokens	*current;
	char		*result;

	current = tokens;
	while (current)
	{
		if (current->type == 6)
		{
			result = find_and_replace(current->value);
			free(current->value);
			current->value = result;
		}
		current = current->next;
	}
}

// int main(void)
// {
// 	char *start = "ded          .$USR$VAR ";
// 	char *end = ft_strnstr(start, "$USER", ft_strlen(start));

// 	char *before = ft_strdup_until(start);

// 	printf("Start: %s\nEnd: %s\nBefore: %s\n", start, end, before);
// }
