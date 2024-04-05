#include "../includes/minishell.h"

char	*find_and_replace(char *org_str)
{
	char		*new_str;
	char		*str_before;
	char		*str_env;
	char		*result;

	if (!org_str)
		return (NULL);
	result = ft_strdup("");
	while (*org_str)
	{
		str_before = ft_strdup_until(org_str);
		org_str = find_env(org_str);
		if (!org_str)
		{
			result = ft_strjoin(result, str_before);
			break ;
		}
		str_env = ft_strdup_delimiter_string(++org_str, WHITESPACE_DOLLAR);
		new_str = ft_strjoin(str_before, get_env(str_env));
		org_str += ft_strlen(str_env);
		result = ft_strjoin(result, new_str);
		free_strs(str_before, str_env, new_str);
	}
	return (result);
}

/* expands environment variables in the token list,
replacing them by their value or by a empty string (\0\0) */
void	expand(t_tokens *tokens)
{
	t_tokens	*current;

	current = tokens;
	while (current)
	{
		if (current->type == 6)
			if (ft_strcmp(current->value, "$$"))
				current->value = find_and_replace(current->value);
		current = current->next;
	}
}
// int main() {
//     // Sample tokens with an environment variable
//     t_tokens *tokens = malloc(sizeof(t_tokens));
//     tokens->type = 6;  // Assuming type 6 represents an environment variable
//     tokens->value = "yes it works $USER";  // Environment variable to expand
//     tokens->next = NULL;
// 	tokens->previous = NULL;

//     // Call the expand function to expand environment variables
//     printf("Before expansion: %s\n", tokens->value);
//     expand(tokens);
//     printf("After expansion: %s\n", tokens->value);

//     // Free allocated memory
//     free(tokens->value);
//     free(tokens);

//     return 0;
// }
