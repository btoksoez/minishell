#include "../../includes/minishell.h"

char	*find_and_replace(t_shell *shell, char *org_str)
{
	char		*str_before;
	char		*str_env;
	char		*result;
	char		*env;

	if (!org_str)
		return (NULL);
	result = ft_strdup("");
	while (*org_str)
	{
		str_before = ft_strdup_until(org_str);
		org_str = find_env(org_str);
		if (!org_str)
		{
			result = ft_strjoin_free(result, str_before);
			break ;
		}
		str_env = ft_strdup_delimiter_string(++org_str, WHITESPACE_DOLLAR_SINGLE);
		env = get_env(shell, str_env);
		str_before = ft_strjoin_free(str_before, env);
		org_str += ft_strlen(str_env);
		result = ft_strjoin_free(result, str_before);
		free_strs(str_before, str_env, env);
	}
	return (result);
}

/* expands environment variables in the token list,
replacing them by their value or by a empty string (\0\0) */
void	expand(t_shell *shell)
{
	t_tokens	*current;
	char		*temp;

	current = shell->tokens;
	while (current)
	{
		if (current->type == 6 || (current->type == 6 && current->previous && current->previous->type != HEREDOC))
		{
			// if (current->previous)
			// 	fprintf(stderr, "current pervious: %s %d\n", current->previous->value, current->previous->type);
			if (ft_strcmp(current->value, "$$"))
			{
				temp = current->value;
				current->value = find_and_replace(shell, temp);
				free(temp);
			}
		}
		current = current->next;
	}
}
