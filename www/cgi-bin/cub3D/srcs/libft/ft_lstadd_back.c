/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 22:40:12 by alerusso          #+#    #+#             */
/*   Updated: 2024/12/23 23:32:03 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <malloc.h>
#include "libft.h"
#define UN_BOTTO 100

void	ft_lstadd_back(t_typelist **lst, t_typelist *new)
{
	t_typelist	*pointer;

	if ((lst) && (*lst))
	{
		pointer = ft_lstlast(*lst);
		pointer->next = new;
	}
	else
		*lst = new;
}
