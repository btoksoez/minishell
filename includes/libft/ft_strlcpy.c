/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btoksoez <btoksoez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:57:16 by andre-da          #+#    #+#             */
/*   Updated: 2024/04/17 11:23:35 by btoksoez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (!dest || !src)
		return (0);
	while (src[i] && size > i)
	{
		dest[i] = src[i];
		i++;
	}
	if (size > 0)
		dest[i] = '\0';
	while (src[i])
		i++;
	return (i);
}

// int	main(void)
// {
// 	char	src[] = "Hello World!";
// 	char	dest[] = "";

// 	size_t	result = ft_strlcpy(dest, src, 7);
// 	printf("length: %zu\n", result);
// 	printf("String: %s\n", dest);
// 	return (0);
// }
