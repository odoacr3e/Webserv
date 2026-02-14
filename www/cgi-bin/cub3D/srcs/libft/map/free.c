/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 01:19:03 by alerusso          #+#    #+#             */
/*   Updated: 2026/02/09 14:54:31 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map_internal.h"

void 	map_free(t_map *ptr)
{
	map_clear(ptr);
	FREE(ptr->values);
	*ptr = (t_map){0};
}

void	_map_default_del(t_map_val *pair)
{
	FREE(pair->key);
	FREE(pair);
}
