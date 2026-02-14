/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 15:55:07 by alerusso          #+#    #+#             */
/*   Updated: 2025/08/28 17:39:02 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cub3D_bonus.h"

static bool	check_one(t_entity *ent, int pos[2], bool discard_open_door);

//	given x, y and an entity type, it returns the index of the entity
//	in its array.
void	*which_entity(t_data *data, int x, int y)
{
	t_entity	*entity;
	int			i;
	int			type;

	type = entity_type(data, x, y);
	if (type == ENTITY_DOOR)
		entity = data->doors;
	else if (type == ENTITY_ENEMY)
		entity = data->enemies;
	else if (type == ENTITY_ITEM)
		entity = data->items;
	else
		return (NULL);
	i = 0;
	while (entity[i].type != ENTITY_END)
	{
		if (entity[i].map[X] == x && entity[i].map[Y] == y)
			break ;
		++i;
	}
	if (entity[i].type == ENTITY_END)
		return (NULL);
	return ((void *)&entity[i]);
}

bool	collision_entity(t_data *data, int x, int y, bool discard_open_door)
{
	if (check_one(data->doors, (int [2]){x, y}, discard_open_door))
		return (true);
	if (check_one(data->enemies, (int [2]){x, y}, discard_open_door))
		return (true);
	if (check_one(data->items, (int [2]){x, y}, discard_open_door))
		return (true);
	return (false);
}

static bool	check_one(t_entity *ent, int pos[2], bool discard_open_door)
{
	int	i;

	if (!ent)
		return (false);
	i = -1;
	while (ent[++i].type != ENTITY_END)
	{
		if (ent[i].map[X] == pos[X] && ent[i].map[Y] == pos[Y])
		{
			if (ent[i].type == DOOR_OPEN && discard_open_door)
				return (false);
			return (true);
		}
	}
	return (false);
}

int	entity_type(t_data *data, int x, int y)
{
	if (ft_strchr(PLAYER_CHARS, data->map[y][x]))
		return (data->player.type);
	if (data->map[y][x] == 'D')
		return (ENTITY_DOOR);
	else if (data->map[y][x] == 'F')
		return (ENTITY_ENEMY);
	else if (data->map[y][x] == 'C')
		return (ENTITY_ITEM);
	return (ENTITY_NOT_FOUND);
}
