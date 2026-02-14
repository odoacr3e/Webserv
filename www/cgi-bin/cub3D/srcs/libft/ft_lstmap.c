/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 22:41:03 by alerusso          #+#    #+#             */
/*   Updated: 2025/08/21 14:56:16 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <malloc.h>
#include "libft.h"
#define UN_BOTTO 100
#define ONLY_HEAD 0
#define EVERYTHING 1

t_typelist		*free_newlist(t_typelist **lst, \
void (*del)(void *), int how_much);
static void		ft_lstiter_custom(t_typelist *lst, void *(*f)(void *));

t_typelist	*ft_lstmap(t_typelist *lst, void *(*f)(void *), void (*del)(void *))
{
	t_typelist	*new_lst;
	t_typelist	*new_node;

	if (!lst)
		return (NULL);
	ft_lstiter_custom(lst, *f);
	new_lst = ft_lstnew((void *)lst->content, lst->type);
	if ((!lst) || (!new_lst))
		return (free_newlist(&new_lst, *del, ONLY_HEAD));
	if (lst->next)
	{
		new_node = NULL;
		lst = lst->next;
		while (lst)
		{
			new_node = ft_lstnew((void *)lst->content, lst->type);
			if (!new_node)
				return (free_newlist(&new_lst, *del, EVERYTHING));
			ft_lstadd_back(&new_lst, new_node);
			lst = lst->next;
		}
	}
	ft_lstiter_custom(new_lst, *f);
	return (new_lst);
}

t_typelist	*free_newlist(t_typelist **lst, void (*del)(void *), int how_much)
{
	if (how_much == ONLY_HEAD)
	{
		if (*lst)
			free((*lst));
	}
	else if (how_much == EVERYTHING)
	{
		ft_lstclear(lst, *del);
	}
	(*lst) = NULL;
	return (NULL);
}

static void	ft_lstiter_custom(t_typelist *lst, void *(*f)(void *))
{
	if (lst)
	{
		while (lst->next)
		{
			if (lst->content)
				lst->content = f(lst->content);
			if (lst->type)
				lst->type = f(lst->type);
			lst = lst->next;
		}
		if (lst->content)
			lst->content = f(lst->content);
		if (lst->type)
			lst->type = f(lst->type);
	}
}
