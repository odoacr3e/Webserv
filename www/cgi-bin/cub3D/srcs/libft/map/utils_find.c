/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_find.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 01:37:21 by alerusso          #+#    #+#             */
/*   Updated: 2026/02/09 14:12:53 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map_internal.h"

// returns true if found
// else false
bool	_map_find(t_map *ptr, const char *key, void *new_val, t_map_find_val *val)
{
	val->i = ptr->hasher(key) % ptr->hash_size;
	val->pair.key = (char *)(void *)key;
	val->pair.val = new_val;
	if (!ptr->values[val->i])
		return (false);
	val->head = ptr->values[val->i];
	val->node = val->head;
	while (val->node)
	{
		if (ft_strncmp(key, ((t_map_val *)val->node->content)->key, INT_MAX) == 0)
			break ;
		val->node = val->node->next;
	}
	if (!val->node)
		return (false);
	val->pair.val = ((t_map_val *)val->node->content)->val;
	return (true);
}

// returns true if found
// else false
bool	map_find(t_map *ptr, const char *key)
{
	t_map_find_val	val;

	val = (t_map_find_val){0};
	return (_map_find(ptr, key, NULL, &val));
}
