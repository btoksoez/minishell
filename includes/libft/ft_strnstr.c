/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btoksoez <btoksoez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 11:17:17 by andre-da          #+#    #+#             */
/*   Updated: 2024/04/22 11:08:44 by btoksoez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *phrase, const char *word, size_t n)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (word[j] == '\0')
		return ((char *)&phrase[i]);
	while (phrase[i] != '\0' && i < n)
	{
		while (phrase[j + i] == word[j] && (i + j < n))
		{
			if (word[j + 1] == '\0')
				return ((char *)&phrase[i]);
			j++;
		}
		j = 0;
		i++;
	}
	return (NULL);
}

/* int	main(void)
{
	char	word[] = "how";
	char	phrase[] = "Hello world, how are you?";

	printf("%s\n", ft_strnstr(phrase, word, 16));
	return (0);
}
 */
