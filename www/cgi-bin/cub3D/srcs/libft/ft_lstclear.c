/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 22:40:44 by alerusso          #+#    #+#             */
/*   Updated: 2024/12/23 23:31:55 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <malloc.h>
#include "libft.h"
#define UN_BOTTO 100

/*
int	main(void)
{
	char	*data;
	t_typelist	*node;
	t_typelist	*node_pointer;
	t_typelist	*new_pointer;
	size_t	size_of_list;
	size_t	counter;

	size_of_list = 20;
	counter = 0;
	data = (char *)malloc(UN_BOTTO);
	if (!data)
		return (2);
	ft_strlcpy(data, (const char *)"MEGA_RAYQUAZA E GABIBBO", UN_BOTTO);
	if (counter < size_of_list)
		node = ft_lstnew((void *)data);
	else
		node = NULL;
	if (!node)
	{
		free(data);
		data = NULL;
		return (2);
	}
	node_pointer = node;
	++counter;
	while ((counter < size_of_list) && (node_pointer))
	{
		new_pointer = ft_lstnew((void *)data);
		if (new_pointer)
		{
			ft_lstadd_front(&node_pointer, new_pointer);
		}
		else
		{
			break ;
		}
		node_pointer = new_pointer;
		++counter;
	}
	node_pointer->next = NULL;
	printf("Contenuto: %s", (char *)node->content);
	printf("\nNext: %p", node->next);
	printf("\nSIZE: %d", ft_lstsize(node));
	ft_strlcpy(node_pointer->content, (char *)"Sono l'ultimo!", UN_BOTTO);
	printf("\nLAST: %s\n", (char *)ft_lstlast(node)->content);
	free(data);
	data = NULL;
	return (0);
}*/

void	ft_lstclear(t_typelist **lst, void (*del)(void *))
{
	t_typelist	*node_pointer;

	if (lst)
	{
		while (*lst)
		{
			node_pointer = (*lst);
			(*lst) = (*lst)->next;
			ft_lstdelone(node_pointer, *del);
		}
		ft_lstdelone((*lst), *del);
	}
}
