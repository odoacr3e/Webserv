/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 15:48:36 by alerusso          #+#    #+#             */
/*   Updated: 2026/02/12 16:26:27 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cub3D_bonus.h"

static char	entity_line(t_data *data, t_entity *ray, double angle);
static char	the_entity_checker(t_data *data, t_entity *ray);
static void	interactions(t_data *data, t_entity *entity);
static void	other_interactions(t_data *data);

void	interact(t_data *data)
{
	t_entity	*entity;
	int			x;
	int			y;
	double		angle;

	if (data->menu || data->battle)
		return (other_interactions(data));
	angle = data->player.pov[X];
	entity_line(data, &data->player, angle);
	x = data->player.curr_x / WIMG;
	y = data->player.curr_y / HIMG;
	if (ft_strchr(WALL_CHARS, data->map[y][x]))
		return ;
	entity = which_entity(data, x, y);
	if (!entity || \
ray_lenght(data, data->player.curr_x, data->player.curr_y) >= 150)
		return ;
	interactions(data, entity);
}

static	char	entity_line(t_data *data, t_entity *ray, double angle)
{
	double	pos[2];
	float	cos_angle;
	float	sin_angle;
	char	ent_type;

	pos[X] = ray->screen[X];
	pos[Y] = ray->screen[Y];
	ray->curr_x = pos[X];
	ray->curr_y = pos[Y];
	if (angle > (2 * PI))
		angle -= (2 * PI);
	else if (angle < 0)
		angle += (2 * PI);
	cos_angle = round_rad(cos(angle));
	sin_angle = round_rad(sin(angle)) * -1;
	ent_type = the_entity_checker(data, ray);
	while (!ent_type)
	{
		pos[X] += cos_angle;
		pos[Y] += sin_angle;
		ray->curr_x = pos[X];
		ray->curr_y = pos[Y];
		ent_type = the_entity_checker(data, ray);
	}
	return (ent_type);
}

static char	the_entity_checker(t_data *data, t_entity *ray)
{
	int	x;
	int	y;

	x = (int)ray->curr_x / WIMG;
	y = (int)ray->curr_y / HIMG;
	if (data->player.map[X] == x && data->player.map[Y] == y)
		return (false);
	if (ft_strchr(WALL_CHARS, data->map[y][x]))
	{
		return ('1');
	}
	return (data->map[y][x]);
}

static void	interactions(t_data *data, t_entity *entity)
{
	if (entity->type == DOOR_CLOSE)
	{
		entity->type = DOOR_OPEN;
		entity->frames[0] = &data->txtr[DOOR_OPEN];
	}
	else if (entity->type == DOOR_OPEN)
	{
		entity->type = DOOR_CLOSE;
		entity->frames[0] = &data->txtr[DOOR_CLOSE];
	}
	else if (entity->type == ENTITY_ENEMY)
	{
		data->battle = true;
		data->foe = entity;
	}
	else if (entity->type == ENTITY_ITEM)
		update_map(data, entity, -1, -1);
}

static void	other_interactions(t_data *data)
{
	if (data->menu)
	{
		if (data->player.input & UP)
		{
			ft_memcpy(&data->events[3], SFX_GAME, ft_strlen(SFX_GAME));
			data->menu = false;
			data->button = 0;
		}
		else if (data->player.input & DOWN)
			data->menu = -1;
	}
}
/*

*/
