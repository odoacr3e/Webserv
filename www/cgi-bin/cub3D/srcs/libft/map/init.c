/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 23:20:43 by alerusso          #+#    #+#             */
/*   Updated: 2026/02/09 12:32:29 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map_internal.h"

int 	map_init(t_map *ptr, size_t hash_size)
{
	*ptr = (t_map){0};
	if (hash_size == 0 || hash_size + 1 == 0)
		return (_map_error(MAP_INIT_EINVAL, &hash_size));
	ptr->values = ft_calloc(hash_size + 1, sizeof(t_list));
	if (!ptr->values)
		return (_map_error(MAP_NOMEM, NULL));
	ptr->alloc = _map_default_alloc;
	ptr->del = _map_default_del;
	ptr->hasher = _map_default_hash;
	ptr->hash_size = hash_size;
	return (0);
}

int	_map_default_alloc(t_map_val *pair, char *key, void *val)
{
	pair->key = ft_strdup(key);
	pair->val = val;
	if (!pair->key)
		return (FREE(pair->key), 1);
	return (0);
}
