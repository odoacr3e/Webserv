/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 23:20:51 by alerusso          #+#    #+#             */
/*   Updated: 2026/02/09 15:00:28 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map_internal.h"

void 	map_clear(t_map *ptr)
{
	for (size_t i = 0; i != ptr->hash_size; i++)
	{
		if (ptr->values[i])
			lst_clear(&ptr->values[i], (void(*)(void*))ptr->del);
	}
}
