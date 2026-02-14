/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_add_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 22:40:12 by alerusso          #+#    #+#             */
/*   Updated: 2025/11/25 23:01:33 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"

void	lst_back(t_list **lst, t_list *new)
{
	t_list	*pointer;

	if (lst && *lst && new)
	{
		pointer = lst_last(*lst);
		pointer->next = new;
		new->prev = pointer;
	}
	else
		*lst = new;
}

void	lst2_back(t_list2 **lst, t_list2 *new)
{
	t_list2	*pointer;

	if (lst && *lst && new)
	{
		pointer = lst2_last(*lst);
		pointer->next = new;
	}
	else
		*lst = new;
}
