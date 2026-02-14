/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_add_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 22:27:45 by alerusso          #+#    #+#             */
/*   Updated: 2025/11/25 23:01:12 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"

void	lst_front(t_list **lst, t_list *new)
{
	if (!new || !lst)
		return ;
	if (*lst)
	{
		new->next = *lst;
		*lst = new;
		new->next->prev = new;
	}
	else
	{
		*lst = new;
		new->next = NULL;
		new->prev = NULL;
	}
}

void	lst2_front(t_list2 **lst, t_list2 *new)
{
	if (!new || !lst)
		return ;
	if (*lst)
	{
		new->next = *lst;
		*lst = new;
	}
	else
	{
		*lst = new;
		new->next = NULL;
	}
}
