/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_utils_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 14:31:50 by alerusso          #+#    #+#             */
/*   Updated: 2026/02/12 16:24:51 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cub3D_bonus.h"

void	save_coord(t_data *data, t_entity *ent, t_entity *ray, double angle);

/*
	//FIXME To optimize line, we should:
	1)	calculate minimal vectors using DDA;
	2)	incrementing curr_x and curr_y by that minimal values;
	3)	calling this function normally;
	4)	stop printing the line.
		At that point, we could differentiate this method using DEBUG macro.

	//REVIEW - collision_checker

	1)	save current minimap position. Then, we check collisions;
	2)	if the position has player or void space, no collisions (FALSE);
	3)	if the position has a wall, TRUE;
	4)	if the position has a door, TRUE;
	5)	if the position has the requested type, TRUE;
	6)	In other case, we save the entity data, and return FALSE.
		Entities are saved in the render array, and print after raycasting.
*/
int	collision_checker(t_entity *ray, t_data *data, double angle, char type)
{
	int			map[2];
	t_entity	*entity;

	map[X] = (int)ray->curr_x / WIMG;
	map[Y] = (int)ray->curr_y / HIMG;
	if (ft_strchr(FFILL_CHARS, data->map[map[Y]][map[X]]))
		return (false);
	if (ft_strchr(WALL_CHARS, data->map[map[Y]][map[X]]))
		return (true);
	if (data->map[map[Y]][map[X]] == 'D')
		return (true);
	if (data->map[map[Y]][map[X]] == type)
		return (true);
	entity = which_entity(data, map[X], map[Y]);
	if (!entity)
		return (false);
	save_coord(data, entity, ray, angle);
	return (false);
}

void	save_coord(t_data *data, t_entity *ent, t_entity *ray, double angle)
{
	int	i;

	if (!ent->contact)
	{
		ent->first_ray = angle;
		i = 0;
		while (data->renderer && data->renderer[i])
			++i;
		data->renderer[i] = ent;
		ent->contact_column = data->column;
		ent->distance = ray_lenght(data, (int)ray->curr_x, (int)ray->curr_y);
		if (ent->distance < 150 && !data->enemy_audio)
		{
			data->enemy_audio = true;
			ft_memcpy(&data->events[3], SFX_ENEMY, ft_strlen(SFX_ENEMY));
		}
		ent->contact = true;
	}
	if (angle != ent->last_ray)
		ent->contact_num += 1;
	ent->last_ray = angle;
}

double	ray_lenght(t_data *data, int rx, int ry)
{
	double	ray;
	int		px;
	int		py;

	px = data->player.screen[X];
	py = data->player.screen[Y];
	rx = abs(rx);
	ry = abs(ry);
	ray = sqrt(pow((double)(rx - px), 2) + pow((double)(ry - py), 2));
	return (ray);
}
