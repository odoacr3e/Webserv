/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_textures_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 09:39:11 by alerusso          #+#    #+#             */
/*   Updated: 2026/02/10 18:44:05 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cub3D_bonus.h"

//static int	check_file(char *texture_name);
bool		check_colors(char *texture, int rgb[3]);

/*REVIEW
Funzione che controlla che i path passati per i file
delle texture siano davvero esistenti, nel caso 
terminiamo gnl con finish him perche' leo non sa scrivere bene i 
programmi e poi restituiamo errore sullo standard error
*/
void	check_textures(t_data *data, int fd)
{
	data->txtr_floor = get_next_line(fd);
	data->txtr_ceiling = get_next_line(fd);
	free(get_next_line(fd));
	if (!data->txtr_floor || !data->txtr_ceiling)
	{
		finish_him(fd);
		error(data, E_COLORS, NULL);
	}
	if (check_colors(data->txtr_floor, data->floor) == false || \
	check_colors(data->txtr_ceiling, data->ceiling) == false)
	{
		finish_him(fd);
		error(data, E_COLORS, NULL);
	}
}

/*
static int	check_file(char *texture_name)
{
	int	fd;

	fd = open(texture_name, O_RDONLY);
	if (fd < 0)
		return (false);
	close(fd);
	fd = open(texture_name, __O_DIRECTORY);
	if (fd < 0)
		return (true);
	close(fd);
	return (false);
}*/

bool	check_colors(char *texture, int rgb[3])
{
	int				i;
	long long int	atoi_result;

	i = -1;
	while (++i != 3)
	{
		if (*texture == ',')
			++texture;
		if (!*texture)
			return (false);
		atoi_result = ft_atoi(texture);
		if (atoi_result == LLONG_MAX)
			return (false);
		if (atoi_result < 0 || atoi_result > 255)
			return (false);
		rgb[i] = (int)atoi_result;
		texture += sub_strlen(texture, ",", EXCLUDE);
	}
	return (true);
}
