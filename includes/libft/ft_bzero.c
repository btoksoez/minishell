/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btoksoez <btoksoez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 13:46:51 by andre-da          #+#    #+#             */
/*   Updated: 2024/04/17 11:16:39 by btoksoez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	size_t	i;
	char	*temp;

	i = 0;
	temp = s;
	if (!s)
		return ;
	if (n == 0)
		return ;
	while (i < n)
	{
		temp[i] = '\0';
		i++;
	}
}

/* int	main(void)
{
	char	str[] = "abc";
	int	i = 0;

	printf("Before: ");
	while (str[i])
	{
		printf("%c", str[i]);
		i++;
	}
	while (str[i])
		i++;
	ft_bzero(str, i);
	i = 0;
	printf("\nAfter: ");
	while (str[i])
	{
		printf("%c", str[i]);
		i++;
	}
	printf("\n");
	return (0);
} */
