/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_insert.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 10:02:36 by alerusso          #+#    #+#             */
/*   Updated: 2026/02/09 13:56:22 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map_internal.h"

int	_map_insert(t_map *ptr, t_map_find_val *val)
{
	t_list		*node;
	t_map_val	*pair;

	pair = ft_calloc(1, sizeof(t_map_val));
	if (!pair)
		return (_map_error(MAP_NOMEM, NULL));
	if (ptr->alloc(pair, val->pair.key, val->pair.val) != 0)
		return (_map_error(MAP_NOMEM, NULL));
	node = lst_new(pair);
	if (!node)
		return (FREE(pair), _map_error(MAP_NOMEM, NULL));
	lst_back(&ptr->values[val->i], node);
	ptr->size++;
	return (0);
}
