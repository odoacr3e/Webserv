/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 22:27:41 by alerusso          #+#    #+#             */
/*   Updated: 2025/12/12 18:23:52 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"

int	lst_size(t_list *lst)
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

int	lst2_size(t_list2 *lst)
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
