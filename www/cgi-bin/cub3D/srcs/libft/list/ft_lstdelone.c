/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_delone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 22:40:30 by alerusso          #+#    #+#             */
/*   Updated: 2025/11/12 19:46:52 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"

void	lst_delone(t_list *lst, void (*del)(void *))
{
	if (del)
		del(lst->content);
	if (lst->next)
		lst->next->prev = lst->prev;
	if (lst->prev)
		lst->prev->next = lst->next;
	FREE(lst);
}

void	lst2_delone(t_list2 *lst, void (*del)(void *))
{
	if (del)
	{
		del(lst->content1);
		del(lst->content2);
	}
	FREE(lst);
}
