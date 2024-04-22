/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btoksoez <btoksoez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 12:50:59 by btoksoez          #+#    #+#             */
/*   Updated: 2024/04/22 12:03:57 by btoksoez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		str_env = ft_strdup_while_string(++org_str, LETTERS_DIGITS);
		env = get_env(shell, str_env);
		str_before = ft_strjoin_free(str_before, env);
		org_str += ft_strlen(str_env);
		result = ft_strjoin_free(result, str_before);
		free_strs(str_before, str_env, env);
	}
	return (result);
}

void	expand_status(t_shell *shell, t_tokens *current)
{
	free(current->value);
	current->value = ft_itoa(shell->status);
}

void	ft_strcpy_exit_status(char *str, char *new_str, char *exit_code)
{
	int	i;
	int	j;

	i = 0;
	while (str[i] != '$')
	{
		new_str[i] = str[i];
		i++;
	}
	j = 0;
	while (exit_code[j] != '\0')
	{
		new_str[i + j] = exit_code[j];
		j++;
	}
	while (str[i + 2] != '\0')
	{
		new_str[i + j] = str[i + 2];
		i++;
	}
	new_str[i + j] = '\0';
}

void	replace_exit_status(t_shell *shell, t_tokens *current)
{
	char	*str;
	char	*replace;
	char	*new_str;
	char	*exit_code;

	str = current->value;
	if (!str)
		return ;
	replace = ft_strnstr(str, "$?", ft_strlen(str));
	if (!replace)
		return ;
	exit_code = ft_itoa(shell->status);
	new_str = (char *)malloc(sizeof(char)
			* (ft_strlen(str) - 1 + ft_strlen(exit_code)));
	if (!new_str)
		return ;
	ft_strcpy_exit_status(str, new_str, exit_code);
	free(current->value);
	free(exit_code);
	current->value = new_str;
}

/* expands environment variables in the token list,
replacing them by their value or by a empty string (\0\0) */
void	expand(t_shell *shell)
{
	t_tokens	*current;
	char		*temp;

	current = shell->tokens;
	temp = NULL;
	while (current)
	{
		if (current->type == 6)
		{
			if (current->previous && current->previous->type == HEREDOC)
			{
			}
			else if (!ft_strcmp(current->value, "$?"))
				expand_status(shell, current);
			else if (ft_strcmp(current->value, "$$"))
			{
				replace_exit_status(shell, current);
				temp = current->value;
				current->value = find_and_replace(shell, temp);
				free(temp);
			}
		}
		current = current->next;
	}
}
