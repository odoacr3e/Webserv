/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_iter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 22:40:56 by alerusso          #+#    #+#             */
/*   Updated: 2025/11/25 22:43:21 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"

void	lst_iter(t_list *lst, void (*f)(void *))
{
	if (lst)
	{
		while (lst->next)
		{
			if (lst->content)
				f(lst->content);
			lst = lst->next;
		}
		if (lst->content)
			f(lst->content);
	}
}

void	lst2_iter(t_list2 *lst, void (*f)(void *))
{
	if (lst)
	{
		while (lst->next)
		{
			if (lst->content1)
				f(lst->content1);
			if (lst->content2)
				f(lst->content2);
			lst = lst->next;
		}
		if (lst->content1)
			f(lst->content1);
		if (lst->content2)
			f(lst->content2);
	}
}
