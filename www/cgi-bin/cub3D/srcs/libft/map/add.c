/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 00:50:00 by alerusso          #+#    #+#             */
/*   Updated: 2026/02/09 12:02:37 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map_internal.h"

int 	map_add(t_map *ptr, char *key, void *new_val)
{
	t_map_find_val	val;

	if (!ptr || !key || !new_val)
		return (_map_error(MAP_EINVAL, NULL));
	if (_map_find(ptr, key, new_val, &val) == true)
		return (_map_error(MAP_EXIST, NULL));
	return (_map_insert(ptr, &val));
}
