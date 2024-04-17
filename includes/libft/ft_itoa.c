/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btoksoez <btoksoez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 12:20:06 by andre-da          #+#    #+#             */
/*   Updated: 2024/04/17 11:18:36 by btoksoez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_length(int f);

char	*ft_itoa(int n)
{
	char	*str;
	int		i;

	i = ft_length(n);
	str = (char *)malloc((i * sizeof(char)) + 1);
	if (str == NULL)
		return (NULL);
	if (n == -2147483648)
	{
		ft_strlcpy(str, "-2147483648", 12);
		return (str);
	}
	if (n < 0)
	{
		str[0] = '-';
		n = -n;
	}
	str[i--] = '\0';
	while (n >= 10)
	{
		str[i--] = (n % 10) + 48;
		n = n / 10;
	}
	str[i] = n + 48;
	return (str);
}

static int	ft_length(int n)
{
	int	i;
	int	c;

	c = n;
	i = 0;
	if (c == -2147483648)
		return (11);
	if (c < 0)
	{
		c = -c;
		i++;
	}
	while (c >= 10)
	{
		i++;
		c = c / 10;
	}
	return (i + 1);
}

/* int	main(void)
{
	printf("%s\n", ft_itoa(-2147483648));
} */
