/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map_access_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:01:16 by lparolis          #+#    #+#             */
/*   Updated: 2025/08/22 08:53:56 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cub3D_bonus.h"

static void	path_finder(t_data *data, char **map_copy, int x, int y);
static void	path_checker(t_data *data, char **map);
static void	player_finder(t_data *data);

/* REVIEW
Questa funzione controlla che tutti punti della mappa siano accessibili dal
player, nel caso non lo siano restituisce errore, per farlo utilizziamo
un flood fill che modifica una copia temporanea della mappa e un checker
successivo che controlla che il floodfill abbia fatto il suo lavoro
 */
void	check_map_access(t_data *data)
{
	char	**map_copy;

	map_copy = ft_dup_matrix(data->map);
	player_finder(data);
	path_finder(data, map_copy, data->player.map[1], data->player.map[0]);
	path_checker(data, map_copy);
	free_matrix(map_copy);
}

static void	path_finder(t_data *data, char **map_copy, int x, int y)
{
	if (map_copy[x][y] == 'X' || map_copy[x][y] == '1')
		return ;
	map_copy[x][y] = 'X';
	path_finder(data, map_copy, x + 1, y);
	path_finder(data, map_copy, x - 1, y);
	path_finder(data, map_copy, x, y + 1);
	path_finder(data, map_copy, x, y - 1);
	return ;
}

static void	player_finder(t_data *data)
{
	int	x;
	int	y;

	x = -1;
	while (data->map[++x])
	{
		y = -1;
		while (data->map[x][++y])
		{
			if (ft_strchr(PLAYER_CHARS, data->map[x][y]))
			{
				data->player.map[X] = y;
				data->player.map[Y] = x;
				data->player.type = data->map[x][y];
			}
		}
	}
	data->player.screen[0] = (data->player.map[0] * WIMG);
	data->player.screen[1] = (data->player.map[1] * HIMG);
}

static void	path_checker(t_data *data, char **map)
{
	int		x;
	int		y;

	x = -1;
	while (map[++x])
	{
		y = -1;
		while (map[x][++y])
		{
			if (ft_strchr(FFILL_CHARS, map[x][y]))
			{
				free_matrix(map);
				error(data, E_INVALID_PATH, NULL);
			}
		}
	}
}
