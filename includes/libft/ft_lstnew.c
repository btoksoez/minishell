/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btoksoez <btoksoez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 15:38:16 by andre-da          #+#    #+#             */
/*   Updated: 2024/04/17 11:19:29 by btoksoez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*nbr;

	if (!content)
		return (NULL);
	nbr = (t_list *)malloc(sizeof(t_list));
	if (nbr == NULL)
		return (NULL);
	nbr->content = content;
	nbr->next = NULL;
	return (nbr);
}

/* int	main(void)
{
	int	data = 42;

	t_list *new_node = ft_lstnew(&data);
	printf("%d\n", *(int *)(new_node->content));
	free(new_node);
	return (0);
} */
