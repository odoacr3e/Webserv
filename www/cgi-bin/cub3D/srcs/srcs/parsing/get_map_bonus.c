/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 17:32:38 by lparolis          #+#    #+#             */
/*   Updated: 2026/02/10 18:49:26 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cub3D_bonus.h"

static void	resize_map(t_data *data);

/* REVIEW
Nella parte iniziale mergiamo tutte le chiamate di gnl in una
singola stringa per poi chiamarci split con \n come carattere separatore
Nella seconda parte resiziamo la mappa per poter riempire gli spazi non allocati
con il carattere spazio
 */
void	get_map(t_data *data, int fd)
{
	char	*pre_split;
	char	*line;

	pre_split = NULL;
	line = get_next_line(fd);
	while (line)
	{
		pre_split = ft_rejoin(pre_split, line, true, true);
		line = get_next_line(fd);
	}
	gnl_statik(fd, true);
	close(fd);
	if (!pre_split)
		return (error(data, E_NO_MAP, NULL));
	data->map = ft_split(pre_split, '\n');
	free(pre_split);
	resize_map(data);
}

/*
	FUNCTION		DESCRIPTION

	check_chars			checks for invalid chars and if there is one player.
	check_walls			checks if all map is surronded by walls.
	check_map_access	checks if all map can be accessed (floodfill).
*/
void	parse_map(t_data *data)
{
	check_chars(data);
	check_walls(data);
	check_other(data);
	check_map_access(data);
}

/*
//REVIEW
	Map needs to be resized because lines length may differ.
	We alloc a new string with size of longest_line - current_line,
	we set it to SPACE, we join it at the current_line.
	Lastly, matrix is updated.
*/
static void	resize_map(t_data *data)
{
	char	**resized_map;
	char	*tmp;
	size_t	max_size;
	size_t	remaining_size;
	int		i;

	i = -1;
	tmp = NULL;
	max_size = matrix_longest_line(data->map);
	data->max_x = max_size - 1;
	resized_map = safe_malloc(sizeof(char *) * (matrix_len(data->map) + 1));
	while (data->map[++i])
	{
		remaining_size = max_size - ft_strlen(data->map[i]);
		tmp = safe_malloc(sizeof(char) * remaining_size + 1);
		tmp = (char *)ft_memset((void *)tmp, ' ', remaining_size);
		tmp[remaining_size] = '\0';
		resized_map[i] = ft_strjoin(data->map[i], tmp);
		free(tmp);
	}
	data->max_y = i - 1;
	resized_map[i] = NULL;
	data->map = ft_rematrix(data->map, resized_map);
}
