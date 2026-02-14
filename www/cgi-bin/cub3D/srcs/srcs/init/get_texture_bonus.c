/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_txtr_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 14:30:58 by alerusso          #+#    #+#             */
/*   Updated: 2025/08/09 15:12:50 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cub3D_bonus.h"
#define MAX_COLORS 32567.0

void		txtr_list(t_data *data);
static void	t_map_delete(t_map_val *pair);
static int	t_map_alloc(t_map_val *pair, char *key, void *val);

/*
//REVIEW	get_txtr

	data->texture is an array of texture of size TEXTURE_NUM + 1.
	we use this to get texture to print.
	
	If one texture is missing, we call error.
*/
void	get_txtr(t_data *data)
{
	int		i;
	t_map	map;

	if (map_init(&map, (int)((MAX_COLORS * 1.3) + 0.5)))
		error(data, E_MALLOC, NULL);
	map.alloc = t_map_alloc;
	map.del = t_map_delete;
	i = -1;
	while (++i != TEXTURES_NUM)
	{
		fill_txtr(data, &map, i);
		map_clear(&map);
		printf("xpm %d/%d\n", i + 1, TEXTURES_NUM);
	}
	i = -1;
	//int	fd = open("Forza_Milan.txt", O_CREAT | O_RDWR, 0666);
	while (++i < TEXTURES_NUM)
	{
		if (!data->txtr[i].xpm)
			return (fd_printf(2, "%d\n", i), error(data, E_MLX_TEXTURE, NULL));
		//fd_printf(fd, "NUMBER %d:\n|%s|\n", i, data->txtr[i].xpm);
	}
	map_free(&map);
	//close(fd);
}

static void	t_map_delete(t_map_val *pair)
{
	free(pair->key);
	free(pair->val);
	free(pair);
}

static int	t_map_alloc(t_map_val *pair, char *key, void *val)
{
	pair->key = ft_strdup(key);
	if (!pair->key)
		return (1);
	pair->val = malloc(5);
	if (!pair->val)
		return (free(pair->key), 1);
	ft_memcpy(pair->val, val, 5);
	return (0);
}

//	set all the txtr array to NULL
void	set_txtr_null(t_data *data)
{
	int	i;

	data->txtr = ft_calloc(TEXTURES_NUM + 1, sizeof(t_txtr));
	if (!data->txtr)
		error(data, E_MALLOC, NULL);
	i = -1;
	while (++i < TEXTURES_NUM)
		data->txtr[i] = (t_txtr){0};
}

/*
//	This function saves the info for the txtr. If something is messed up,
	the program is closed safely.

	1)	the img is parsed by mlx. The char array is stored in the txtr struct;
	2)	we read the img height reading the raw xpm file.
	3)	we save the size of the xpm char * array given by mlx, calculating
		it by taking the size of the xpm.
*/
//void	fill_txtr(t_data *data, int index, int size[2])
void	fill_txtr(t_data *data, t_map *map, int index)
{
	t_txtr	*txtr;

	txtr = &data->txtr[index];
//	txtr->xpm = mlx_get_data_addr(txtr->ptr, &txtr->bpp, &txtr->size[X], &txtr->endian);
	parse_xpm(data, txtr, map, index);
	txtr->shade = 1;
	txtr->scaler[X] = (txtr->size[X] / (txtr->bpp / 8)) / TXTR;
	txtr->scaler[Y] = txtr->size[Y] / TXTR;
	txtr->i = index;
}
