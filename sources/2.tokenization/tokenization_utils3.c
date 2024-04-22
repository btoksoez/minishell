/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_utils3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btoksoez <btoksoez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:52:19 by btoksoez          #+#    #+#             */
/*   Updated: 2024/04/22 12:52:40 by btoksoez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*token_dollar(char *start, t_tokens *token)
{
	char	*word;

	word = ft_strdup_delimiter_string(start, LETTERS_DIGITS_WS);
	if (!word)
		return (error_message("token error: no word", NULL), NULL);
	token->value = word;
	token->type = WORD;
	start += ft_strlen(word);
	if (ft_strchr(WHITESPACE, *start))
		token->space = 1;
	return (start);
}

char	*token_dollar2(char *start, t_tokens *token)
{
	char	*word;

	word = ft_strdup("$");
	if (!word)
		return (error_message("token error: no word", NULL), NULL);
	token->value = word;
	token->type = WORD;
	start += ft_strlen(word);
	if (ft_strchr(WHITESPACE, *start))
		token->space = 1;
	return (start);
}

char	*token_dollarnumber(char *start, t_tokens *token)
{
	char	*word;

	word = ft_strdup("");
	if (!word)
		return (error_message("token error: no word", NULL), NULL);
	token->value = word;
	token->type = WORD;
	start += 2;
	if (ft_strchr(WHITESPACE, *start))
		token->space = 1;
	return (start);
}
