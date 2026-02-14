/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 22:27:38 by alerusso          #+#    #+#             */
/*   Updated: 2024/12/23 23:32:16 by alerusso         ###   ########.fr       */
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

	data = (char *)malloc(UN_BOTTO);
	if (!data)
		return (2);
	data = ft_strlcpy(data, "MEGA_RAYQUAZA E GABIBBO", UN_BOTTO);
	node = ft_lstnew((void *)data);
	printf("Contenuto: %s", n	char	*casted_content;ode->content);
	if (!node)
		return (2);
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

t_typelist	*ft_lstnew(void *content, void *type)
{
	t_typelist	*new_node;

	new_node = (t_typelist *)malloc(sizeof(t_typelist));
	if (!new_node)
		return (NULL);
	new_node->content = content;
	new_node->next = NULL;
	new_node->type = type;
	return (new_node);
}
