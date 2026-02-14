/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 22:27:41 by alerusso          #+#    #+#             */
/*   Updated: 2024/12/23 23:32:31 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <malloc.h>
#include "libft.h"
#define UN_BOTTO 100

/*
int main()
{
	char	*data;
	t_typelist	*node;
	t_typelist	*node_pointer;
	t_typelist	*new_pointer;
	size_t	size_of_list;
	size_t	counter;

	size_of_list = 1;
	counter = 0;
	data = (char *)malloc(UN_BOTTO);
	if (!data)
		return (2);
	data = ft_strlcpy(data, "MEGA_RAYQUAZA E GABIBBO", UN_BOTTO);
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
	printf("Contenuto: %s", node->content);
	printf("\nNext: %p\n", node->next);
	printf("\nSIZE: %d\n", ft_lstsize(node));
	while ((node->next))
	{
		if (node->content)
		{
			free(node->content);
			node->content = NULL;
		}
		node_pointer = node;
		node = node->next;
		if (node_pointer)
		{
			free(node_pointer);
			node_pointer = NULL;
		}
	}
	if (node->content)
	{
		free(node->content);
		node->content = NULL;
	}
	if (node)
	{
		free(node);
		node = NULL;
	}
	free(data);
	data = NULL;
	return (0);
}*/

int	ft_lstsize(t_typelist *lst)
{
	int	list_size;

	list_size = 0;
	if (!lst)
		return (0);
	list_size += 1;
	while ((lst->next))
	{
		lst = lst->next;
		list_size += 1;
	}
	return (list_size);
}
