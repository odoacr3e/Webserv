/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 01:24:15 by alerusso          #+#    #+#             */
/*   Updated: 2026/02/09 12:30:40 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map_internal.h"

void 	_map_del(t_map *ptr, t_map_find_val *val)
{
	if (val->node == val->head)
		ptr->values[val->i] = ptr->values[val->i]->next;
	lst_delone(val->node, (void(*)(void*))ptr->del);
	ptr->size--;
}

void 	map_del(t_map *ptr, char *key)
{
	t_map_find_val	val;

	if (!ptr || !key)
		return (_map_error(MAP_EINVAL, NULL), (void)0);
	if (_map_find(ptr, key, NULL, &val) == false)
		return (_map_error(MAP_NOTFOUND, key), (void)0);
	return (_map_del(ptr, &val));
}
