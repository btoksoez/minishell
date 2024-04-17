/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btoksoez <btoksoez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 18:34:52 by andre-da          #+#    #+#             */
/*   Updated: 2024/04/17 11:28:23 by btoksoez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t				i;
	const unsigned char	*src_p;
	unsigned char		*dest_p;

	i = -1;
	src_p = src;
	dest_p = dest;
	if (!dest || !src)
		return (NULL);
	if (src_p < dest_p)
	{
		while (n > 0)
		{
			n--;
			dest_p[n] = src_p[n];
		}
	}
	else if (src_p > dest_p)
	{
		while (++i < n)
			dest_p[i] = src_p[i];
	}
	return (dest);
}

/* int	main(void
{
	char	src[] = "Hello World!";
	char	dest[1000] = "Hello";

	printf("My Function: %s\n", (char*)ft_memmove(dest, src, 13));
	printf("Function:    %s\n", (char*)memmove(dest, src, 13));
	return (0);
} */
