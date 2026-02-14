/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 11:17:46 by lparolis          #+#    #+#             */
/*   Updated: 2026/02/14 04:52:17 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cub3D_bonus.h"

static void	is_it_valid(t_data *data, int x, int y, int flag);

// printf("char invalido(cioe che non puo camminare): [%c]", data->map[j][i]);
/* REVIEW
Controllo che non ci siano caratteri invalidi all'interno della mappa 
Controllo che il carattere del player non sia ripetuto
Controllo che la mappa sia racchiusa all'interno di un perimetro di muri
Controllo che il player non sia messo ai bordi della mappa
 */
void	check_chars(t_data *data)
{
	int		player_count;

	if (DEBUG == true)
		print_matrix(data->map);
	player_count = 0;
	count_chars(data, &player_count, PLAYER_CHARS);
	if (player_count == 0)
		error(data, E_NO_PLAYER, NULL);
	else if (player_count > 1)
		error(data, E_MULTIPLAYER, NULL);
}

void	check_walls(t_data *data)
{
	int		i;
	int		j;

	i = -1;
	while (data->map && data->map[++i])
	{
		j = -1;
		while (data->map && data->map[i][++j])
		{
			if (ft_strchr(FFILL_CHARS, data->map[i][j]))
				is_it_valid(data, j, i, E_INVALID_MAP);
		}
	}
}

void	check_other(t_data *data)
{
	int		i;
	int		j;

	i = -1;
	while (data->map && data->map[++i])
	{
		j = -1;
		while (data->map && data->map[i][++j])
		{
			if (data->map[i][j] == 'D')
				is_it_valid(data, j, i, E_INVALID_DOOR);
			if (data->map[i][j] == 'F')
				is_it_valid(data, j, i, E_INVALID_ENEMY);
		}
	}
}

static void	is_it_valid(t_data *data, int x, int y, int flag)
{
	if (x == 0 || x == data->max_x || y == 0 || y == data->max_y)
		error(data, flag, NULL);
	if (data->map[y + 1][x] == ' ' || \
data->map[y - 1][x] == ' ' || \
data->map[y][x + 1] == ' ' || \
data->map[y][x - 1] == ' ')
		error(data, flag, NULL);
}
