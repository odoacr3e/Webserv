/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 12:13:44 by alerusso          #+#    #+#             */
/*   Updated: 2026/02/12 08:44:06 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cub3D_bonus.h"

void	one_step(t_data *data, t_entity *entity, double angle[], int offset[]);

/*
	In our game, every entity (player, enemies, objects that moves) is a
	vector.
	When moving, your are just updating your vector data, and moving
	according to those data.

	angle[COS] is the rotation in the x axis, in radiant.
	angle[SIN] is the rotation in the y axis, in radiant.
	the last parameter, offset, is necessary to solve imprecision in
	movement (	in UP and LEFT, the y axis goes one wrong step back;
				in DOWN and RIGHT, the same happens for the x axis).
*/
void	move(t_data *data, t_entity *entity, double angle[])
{
	if (entity->input & UP)
	{
		angle[COS] = cos(entity->pov[X]);
		angle[SIN] = sin(entity->pov[X]) * -1;
		one_step(data, entity, angle, (int [2]){0, 1});
	}
	if (entity->input & LEFT)
	{
		angle[COS] = cos(entity->pov[X] + ANGLE_90);
		angle[SIN] = sin(entity->pov[X] + ANGLE_90) * -1;
		one_step(data, entity, angle, (int [2]){0, 1});
	}
	if (entity->input & DOWN)
	{
		angle[COS] = cos(entity->pov[X]) * -1;
		angle[SIN] = sin(entity->pov[X]);
		one_step(data, entity, angle, (int [2]){1, 0});
	}
	if (entity->input & RIGHT)
	{
		angle[COS] = cos(entity->pov[X] - ANGLE_90);
		angle[SIN] = sin(entity->pov[X] - ANGLE_90) * -1;
		one_step(data, entity, angle, (int [2]){1, 0});
	}
}

/*
	before updating the entity coordinates, we check, with some local temp
	variables, if the player would collide to a wall or another entity.
*/
void	one_step(t_data *data, t_entity *entity, double angle[], int offset[])
{
	double	new_x;
	double	new_y;
	int		map_x;
	int		map_y;

	new_x = entity->screen[X] + (angle[COS] * entity->speed) + offset[X];
	new_y = entity->screen[Y] + (angle[SIN] * entity->speed) + offset[Y];
	map_x = (int)(new_x) / WIMG;
	map_y = (int)(new_y) / HIMG;
	if (map_y < 0 || map_y >= data->max_y || \
map_x < 0 || map_x >= data->max_x)
		return ;
	if (ft_strchr("1 ", data->map[map_y][map_x]) || \
collision_entity(data, map_x, map_y, true) == true)
		return ;
	entity->screen[X] = new_x;
	entity->screen[Y] = new_y;
	update_map(data, entity, map_x, map_y);
	entity->map[X] = map_x;
	entity->map[Y] = map_y;
}

void	rotate(t_data *data, t_entity *entity)
{
	(void)data;
	if (entity->input & R_LEFT)
	{
		entity->pov[X] += ANGULAR_SPEED;
		if (entity->pov[X] > 2 * PI)
			entity->pov[X] -= 2 * PI;
	}
	if (entity->input & R_RIGHT)
	{
		entity->pov[X] -= ANGULAR_SPEED;
		if (entity->pov[X] < 0)
			entity->pov[X] += 2 * PI;
	}
}

/*
	mlx_mouse_get_pos:	store in two variables the current position of the mouse.
	mlx_mouse_move:		set the mouse position in a defined position.

	this function calculates the current distance between 
	the middle of the screen (WSCREEN / 2, HSCREEN / 2) and the mouse position.
	then, it multiplies this difference for RADIANT and for SENSITIVITY.
	Lastly, the mouse is put in the middle of the screen again.
*/
void	mouse_input(t_data *data)
{
	int		pos[2] = {0, 0};
	double	diff[2];

	return ;
//	mlx_mouse_get_pos(data->mlx, data->win, &pos[X], &pos[Y]);
	diff[X] = RADIANT * (pos[X] - (WSCREEN / 2));
	diff[Y] = RADIANT * (pos[Y] - (HSCREEN / 2));
	if (diff[X] == 0 && diff[Y] == 0)
		return ;
	diff[X] *= SENSITIVITY;
	diff[Y] *= SENSITIVITY;
	data->player.pov[X] += (diff[X] * -1);
	if (data->player.pov[X] > PI * 2)
		data->player.pov[X] -= PI * 2;
	else if (data->player.pov[X] < 0)
		data->player.pov[X] += PI * 2;
	//mlx_mouse_move(data->mlx, data->win, WSCREEN / 2, HSCREEN / 2);
}

void	update_map(t_data *data, t_entity *entity, int new_x, int new_y)
{
	static char	temp;

	if (entity->map[X] == new_x && entity->map[Y] == new_y)
		return ;
	if (new_x == -1 && new_y == -1)
	{
		data->map[entity->map[Y]][entity->map[X]] = '0';
		return ;
	}
	else if (data->map[new_y][new_x] == 'D')
	{
		data->map[entity->map[Y]][entity->map[X]] = '0';
		temp = 'D';
	}
	else if (temp == 'D')
	{
		data->map[entity->map[Y]][entity->map[X]] = temp;
		temp = '\0';
	}
	else
	{
		data->map[entity->map[Y]][entity->map[X]] = data->map[new_y][new_x];
	}
	data->map[new_y][new_x] = entity->type;
}
