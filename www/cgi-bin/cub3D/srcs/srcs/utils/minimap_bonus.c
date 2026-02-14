/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 08:45:43 by alerusso          #+#    #+#             */
/*   Updated: 2025/08/25 15:20:21 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cub3D_bonus.h"

static void	minimap_print(t_data *data, int offset[2], int pos[2]);
static void	background(t_data *data);

/*
	i and j:	coordinates of the screen.
	x and y:	map coordinates.
				they are passed as parameter cause of norminette.
				they are set to 1, to skip a block for the border.
				
*/
int	map_start(t_data *data, int i, int j)
{
	int	x;
	int	y;

	background(data);
	y = data->player.map[Y] - ((MINIMAP / 2));
	j = 1;
	if (y < 0)
		y = 0;
	while (j < MINIMAP + 1 && y <= data->max_y)
	{
		i = 1;
		x = data->player.map[X] - ((MINIMAP / 2));
		if (x < 0)
			x = 0;
		while (i < MINIMAP + 1 && data->map[y][x] != ' ' && x <= data->max_x)
		{
			minimap_print(data, (int [2]){i, j}, (int [2]){x, y});
			++x;
			++i;
		}
		++j;
		++y;
	}
	return (0);
}

static void	background(t_data *data)
{
	int	x;
	int	y;

	if (TEXTURES_NUM <= M_BACKGROUND)
		return ;
	x = data->txtr[M_BACKGROUND].size[X] / (data->txtr[M_BACKGROUND].bpp / 8);
	y = data->txtr[M_BACKGROUND].size[Y];
	put_image_resize(data, M_BACKGROUND, (int [2]){0, 0}, (int [2]){x, y});
}

static void	minimap_print(t_data *data, int offset[2], int pos[2])
{
	int			size_minimap[2];
	t_entity	*ent;

	size_minimap[X] = WIMG_MINIMAP;
	size_minimap[Y] = HIMG_MINIMAP;
	if (data->map[pos[Y]][pos[X]] == '1')
		put_image_resize(data, WALL, offset, size_minimap);
	else if (ft_strchr(PLAYER_CHARS, data->map[pos[Y]][pos[X]]))
		put_image_resize(data, which_p(data), offset, size_minimap);
	else if (data->map[pos[Y]][pos[X]] == 'C')
		put_image_resize(data, M_COIN, offset, size_minimap);
	else if (data->map[pos[Y]][pos[X]] == 'F')
		put_image_resize(data, M_FOE, offset, size_minimap);
	ent = which_entity(data, pos[X], pos[Y]);
	if (!ent)
		return ;
	if (ent->type == DOOR_CLOSE)
		put_image_resize(data, M_DOOR, offset, size_minimap);
}
