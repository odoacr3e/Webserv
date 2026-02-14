/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 01:22:21 by alerusso          #+#    #+#             */
/*   Updated: 2026/02/09 12:08:50 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map_internal.h"

int map_replace(t_map *ptr, char *key, void *new_val)
{
	t_map_find_val	val;

	if (!ptr || !key || !new_val)
		return (_map_error(MAP_EINVAL, NULL));
	if (_map_find(ptr, key, new_val, &val) == true)
		_map_del(ptr, &val);
	return (_map_insert(ptr, &val));
}
