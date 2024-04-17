/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btoksoez <btoksoez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 23:17:00 by andrealbuqu       #+#    #+#             */
/*   Updated: 2024/04/17 11:18:36 by btoksoez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list	*lst)
{
	int	length;

	if (lst == NULL)
		return (0);
	length = 1;
	while (lst->next != NULL)
	{
		lst = lst->next;
		length++;
	}
	return (length);
}

/* int main(void)
{
	int	data = 42;

	t_list *new_node = ft_lstnew(&data);
	printf("%d\n", ft_lstsize(new_node));
	free(new_node);
	return (0);
} */
