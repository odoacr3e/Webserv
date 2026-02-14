/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:27:19 by alerusso          #+#    #+#             */
/*   Updated: 2025/08/28 09:49:43 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cub3D_bonus.h"

static void	initialize(t_data *data, t_entity *entity, int pos[2], char c);

/*
	set player struct data.
	screen:		the position in the mini map, with pixel precision
	curr_x/y:	a temp variable used in raycasting to check pixels
	speed:		the amount of movement for every frame
	pov:		the direction, in radiant, which the player points to

	init_animation saves in player an array of two integer.
	those integer are the texture index used for every animation frame.
*/
void	init_player(t_data *data)
{
	data->player.screen[X] = (data->player.map[X] * WIMG) + (WIMG / 2);
	data->player.screen[Y] = (data->player.map[Y] * HIMG) + (HIMG / 2);
	data->player.curr_x = data->player.screen[X];
	data->player.curr_y = data->player.screen[Y];
	data->player.speed = 5;
	if (data->player.type == 'E')
		data->player.pov[X] = RADIANT * (0);
	else if (data->player.type == 'N')
		data->player.pov[X] = RADIANT * (90);
	else if (data->player.type == 'W')
		data->player.pov[X] = RADIANT * (180);
	else if (data->player.type == 'S')
		data->player.pov[X] = RADIANT * (270);
	init_animation(data, &data->player, 2, ARMS1);
	data->player.type = 'N';
}

//	doors are allocated in an array of doors.
//	the data struct is the same as the player (t_entity).
void	init_entity(t_data *data, t_entity **entity, int n, char c)
{
	int		y;
	int		x;
	int		i;

	(*entity) = ft_calloc((n + 2), sizeof(t_entity));
	if (!(*entity))
		error(data, E_MALLOC, NULL);
	i = 0;
	y = -1;
	while (data->map && data->map[++y])
	{
		x = -1;
		while (data->map && data->map[y][++x])
		{
			if (data->map[y][x] == c)
			{
				initialize(data, &((*entity)[i]), (int [2]){x, y}, c);
				i++;
			}
		}
	}
	(*entity)[i] = (t_entity){0};
}

//	initialize every entity.
static void	initialize(t_data *data, t_entity *entity, int pos[2], char c)
{
	*entity = (t_entity){0};
	entity->map[X] = pos[X];
	entity->map[Y] = pos[Y];
	entity->screen[X] = (pos[X] * WIMG) + (WIMG / 2);
	entity->screen[Y] = (pos[Y] * HIMG) + (HIMG / 2);
	entity->speed = 4;
	entity->pov[X] = 0;
	entity->contact_num = 0;
	if (c == 'D')
	{
		entity->type = DOOR_CLOSE;
		init_animation(data, entity, 1, DOOR_CLOSE);
	}
	else if (c == 'F')
	{
		entity->type = ENTITY_ENEMY;
		init_animation(data, entity, 4, FOE_BIT1);
	}
	else
	{
		entity->type = ENTITY_ITEM;
		init_animation(data, entity, 1, COIN);
	}
}
