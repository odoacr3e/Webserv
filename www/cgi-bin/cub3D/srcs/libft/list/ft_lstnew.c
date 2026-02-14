/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 22:27:38 by alerusso          #+#    #+#             */
/*   Updated: 2025/12/12 18:09:57 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"

t_list	*lst_new(void *content)
{
	t_list	*new_node;

	new_node = (t_list *)MALLOC(sizeof(t_list));
	if (!new_node)
		return (NULL);
	new_node->content = content;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

t_list2	*lst2_new(void *content1, void *content2)
{
	t_list2	*new_node;

	new_node = (t_list2 *)MALLOC(sizeof(t_list2));
	if (!new_node)
		return (NULL);
	new_node->content1 = content1;
	new_node->content2 = content2;
	new_node->next = NULL;
	return (new_node);
}
