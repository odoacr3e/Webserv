/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render3_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:39:23 by alerusso          #+#    #+#             */
/*   Updated: 2025/08/28 17:43:42 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cub3D_bonus.h"

void	render_cross(t_data *data)
{
	int	pos[2];

	pos[X] = (WSCREEN / 2) - (data->txtr[CROSSHAIR].size[X] / 8);
	pos[Y] = (HSCREEN / 2) - (data->txtr[CROSSHAIR].size[Y] / 2);
	put_image_to_image(data, CROSSHAIR, pos, (int [2]){WIMG, HIMG});
}

void	render_arms(t_data *data, t_entity *player)
{
	int	pos[2];
	int	size[2];

	if (player->input & MOVEMENT)
		update_animation(player);
	size[X] = player->frames[player->f_curr]->size[X] / 4;
	size[Y] = player->frames[player->f_curr]->size[Y];
	pos[X] = (WSCREEN / 2) - (size[X] / 2);
	pos[Y] = 700;
	put_image_to_image(data, player->frames[player->f_curr]->i, pos, size);
}

/*
	Put an image to the screen texture.
*/
void	put_image_to_image(t_data *data, int which, int pos[2], int size[2])
{
	t_txtr		*txtr;
	int			color;
	int			index;
	int			y;
	int			x;

	txtr = &data->txtr[which];
	y = 0;
	while (y != size[Y])
	{
		x = 0;
		while (x != size[X])
		{
			index = y * txtr->size[X] + x * (txtr->bpp / 8);
			if (index > txtr->total_size - 1)
				return ;
			color = get_pixel_color(&data->txtr[which], index);
			put_pixel(data, pos[X] + x, pos[Y] + y, color);
			++x;
		}
		y++;
	}
}

void	put_image_resize(t_data *data, int which, int pos[2], int size[2])
{
	int	x;
	int	y;

	x = pos[X] * size[X];
	y = pos[Y] * size[Y];
	put_image_to_image(data, which, (int [2]){x, y}, size);
}

/*
//REVIEW - sort_matrix

	A bubble sort algorithm to sort a matrix of strings in 
	ASCII ascending order.
*/
void	sort_render(t_entity **renderer)
{
	t_entity	*temp;
	int			i;
	int			j;

	if (!renderer || !renderer[0])
		return ;
	i = 0;
	while (renderer[i + 1])
	{
		j = 0;
		while (renderer[j + 1])
		{
			if (renderer[j]->distance < renderer[j + 1]->distance)
			{
				temp = renderer[j];
				renderer[j] = renderer[j + 1];
				renderer[j + 1] = temp;
			}
			++j;
		}
		++i;
	}
}
