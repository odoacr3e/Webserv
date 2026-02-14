/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_select.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 14:01:04 by alerusso          #+#    #+#             */
/*   Updated: 2025/08/29 09:58:11 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cub3D_bonus.h"

int	which_p(t_data *data)
{
	double	pov;
	int		i;

	pov = fabs(data->player.pov[X]);
	if (pov > ANGLE_360 - ANGLE_1 || pov < ANGLE_1)
	{
		return (M_PLAYER_0);
	}
	i = 1;
	while (i < 18)
	{
		if (pov < ANGLE_20 * i)
			return (M_PLAYER_0 + i);
		++i;
	}
	return (M_PLAYER_340);
}

t_txtr	*texture_finder(t_data *data, double ray_angle, int hit_x, int hit_y)
{
	t_entity	*entity;

	if (collision_entity(data, hit_x / WIMG, hit_y / HIMG, false) == true)
	{
		entity = which_entity(data, hit_x / WIMG, hit_y / HIMG);
		if (!entity)
			return (&data->txtr[ARMS1]);
		return (entity->frames[entity->f_curr]);
	}
	else
		return (&data->txtr[wall_face(data, &data->player, ray_angle)]);
}

int	texture_x_offset(t_data *data, double ray_angle, int hit_x, int hit_y)
{
	int			wall_txtr;
	int			pixel;

	wall_txtr = wall_face(data, &data->player, ray_angle);
	if (wall_txtr == NORTH || wall_txtr == SOUTH)
		pixel = (int)((((double)hit_x / WIMG) - (int)(hit_x / WIMG)) * TXTR);
	else
		pixel = (int)((((double)hit_y / WIMG) - (int)(hit_y / WIMG)) * TXTR);
	if (wall_txtr == WEST || wall_txtr == SOUTH)
		pixel = TXTR - pixel;
	return (pixel * (data->txtr[SCREEN].bpp / 8));
}
