/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstget.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 15:33:02 by alerusso          #+#    #+#             */
/*   Updated: 2025/12/12 18:40:04 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"

void	*lst_get(t_list *lst, int32_t n)
{
	if (!lst || n < 0)
		return (NULL);
	while (n-- && lst)
		lst = lst->next;
	return (lst);
}

void	*lst2_get(t_list2 *lst, int32_t n)
{
	if (!lst || n < 0)
		return (NULL);
	while (n-- && lst)
		lst = lst->next;
	return (lst);
}
