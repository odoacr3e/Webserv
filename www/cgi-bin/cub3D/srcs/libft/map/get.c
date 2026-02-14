/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 23:20:46 by alerusso          #+#    #+#             */
/*   Updated: 2026/02/09 12:42:12 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map_internal.h"

void 	*map_get(t_map *ptr, char *key)
{
	t_map_find_val	val;

	if (!ptr || !key)
		return (_map_error(MAP_EINVAL, NULL), NULL);
	if (_map_find(ptr, key, NULL, &val) == false)
		return (_map_error(MAP_NOTFOUND, key), NULL);
	return (val.pair.val);
}
